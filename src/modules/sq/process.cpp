// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2020 Team Dissolve and contributors

#include "classes/box.h"
#include "classes/configuration.h"
#include "genericitems/listhelper.h"
#include "main/dissolve.h"
#include "math/averaging.h"
#include "modules/rdf/rdf.h"
#include "modules/sq/sq.h"

// Run main processing
bool SQModule::process(Dissolve &dissolve, ProcessPool &procPool)
{
    /*
     * Calculate S(Q) from Configuration's g(r).
     *
     * This is a serial routine, with each process constructing its own copy of the data.
     */

    // Check for zero Configuration targets
    if (targetConfigurations_.nItems() == 0)
        return Messenger::error("No configuration targets set for module '{}'.\n", uniqueName());

    const auto averaging = keywords_.asInt("Averaging");
    auto averagingScheme = keywords_.enumeration<Averaging::AveragingScheme>("AveragingScheme");
    const auto &qBroadening = keywords_.retrieve<BroadeningFunction>("QBroadening", BroadeningFunction());
    const auto qDelta = keywords_.asDouble("QDelta");
    const auto qMin = keywords_.asDouble("QMin");
    double qMax = keywords_.asDouble("QMax");
    if (qMax < 0.0)
        qMax = 30.0;
    const bool saveData = keywords_.asBool("Save");
    const auto &windowFunction = keywords_.retrieve<WindowFunction>("WindowFunction", WindowFunction());

    // Print argument/parameter summary
    Messenger::print("SQ: Calculating S(Q)/F(Q) over {} < Q < {} Angstroms**-1 using step size of {} Angstroms**-1.\n", qMin,
                     qMax, qDelta);
    if (averaging <= 1)
        Messenger::print("SQ: No averaging of partials will be performed.\n");
    else
        Messenger::print("SQ: Partials will be averaged over {} sets (scheme = {}).\n", averaging,
                         Averaging::averagingSchemes().keyword(averagingScheme));
    if (windowFunction.function() == WindowFunction::NoWindow)
        Messenger::print("SQ: No window function will be applied in Fourier transforms of g(r) to S(Q).");
    else
        Messenger::print("SQ: Window function to be applied in Fourier transforms is {} ({}).",
                         WindowFunction::functionType(windowFunction.function()), windowFunction.parameterSummary());
    if (qBroadening.function() == BroadeningFunction::NoFunction)
        Messenger::print("SQ: No broadening will be applied to calculated S(Q).");
    else
        Messenger::print("SQ: Broadening to be applied in calculated S(Q) is {} ({}).",
                         BroadeningFunction::functionType(qBroadening.function()), qBroadening.parameterSummary());
    Messenger::print("SQ: Save data is {}.\n", DissolveSys::onOff(saveData));
    Messenger::print("\n");

    /*
     * Loop over target Configurations and Fourier transform their UnweightedGR into the UnweightedSQ.
     */
    for (Configuration *cfg : targetConfigurations_)
    {
        // Set up process pool - must do this to ensure we are using all available processes
        procPool.assignProcessesToGroups(cfg->processPool());

        // Get unweighted g(r) for this Configuration - we don't supply a specific Module prefix, since the unweighted
        // g(r) may come from one of many RDF-type modules
        if (!cfg->moduleData().contains("UnweightedGR"))
            return Messenger::error("Couldn't locate UnweightedGR for Configuration '{}'.\n", cfg->name());
        const auto &unweightedgr = GenericListHelper<PartialSet>::value(cfg->moduleData(), "UnweightedGR");

        // Does a PartialSet already exist for this Configuration?
        bool wasCreated;
        auto &unweightedsq = GenericListHelper<PartialSet>::realise(cfg->moduleData(), "UnweightedSQ", uniqueName(),
                                                                    GenericItem::InRestartFileFlag, &wasCreated);
        if (wasCreated)
            unweightedsq.setUpPartials(unweightedgr.atomTypes(), fmt::format("{}-{}", cfg->niceName(), uniqueName()),
                                       "unweighted", "sq", "Q, 1/Angstroms");

        // Is the PartialSet already up-to-date?
        if (DissolveSys::sameString(unweightedsq.fingerprint(), fmt::format("{}", cfg->moduleData().version("UnweightedGR"))))
        {
            Messenger::print("SQ: Unweighted partial S(Q) are up-to-date for Configuration '{}'.\n", cfg->name());
            continue;
        }

        // Transform g(r) into S(Q)
        if (!calculateUnweightedSQ(procPool, cfg, unweightedgr, unweightedsq, qMin, qDelta, qMax, cfg->atomicDensity(),
                                   windowFunction, qBroadening))
            return false;

        // Perform averaging of unweighted partials if requested, and if we're not already up-to-date
        if (averaging > 1)
        {
            // Store the current fingerprint, since we must ensure we retain it in the averaged T.
            std::string currentFingerprint{unweightedsq.fingerprint()};

            Averaging::average<PartialSet>(cfg->moduleData(), "UnweightedSQ", uniqueName(), averaging, averagingScheme);

            // Need to rename data within the contributing datasets to avoid clashes with the averaged data
            for (int n = averaging; n > 0; --n)
            {
                if (!cfg->moduleData().contains(fmt::format("UnweightedSQ_{}", n), uniqueName()))
                    continue;
                auto &p = GenericListHelper<PartialSet>::retrieve(cfg->moduleData(), fmt::format("UnweightedSQ_{}", n));
                p.setObjectTags(fmt::format("{}//{}//UnweightedSQ", cfg->niceName(), uniqueName()), fmt::format("Avg{}", n));
            }

            // Re-set the object names and fingerprints of the partials
            unweightedsq.setFingerprint(currentFingerprint);
        }

        // Set names of resources (Data1D) within the PartialSet
        unweightedsq.setObjectTags(fmt::format("{}//{}//{}", cfg->niceName(), uniqueName(), "UnweightedSQ"));
        unweightedsq.setFingerprint(fmt::format("{}", cfg->moduleData().version("UnweightedGR")));

        // Save data if requested
        if (saveData && configurationLocal_ && (!MPIRunMaster(procPool, unweightedsq.save())))
            return false;
    }

    // Create/retrieve PartialSet for summed partial S(Q)
    auto &summedUnweightedSQ = GenericListHelper<PartialSet>::realise(dissolve.processingModuleData(), "UnweightedSQ",
                                                                      uniqueName_, GenericItem::InRestartFileFlag);

    // Sum the partials from the associated Configurations
    if (!sumUnweightedSQ(procPool, this, dissolve.processingModuleData(), summedUnweightedSQ))
        return false;

    // Create/retrieve PartialSet for summed unweighted g(r)
    auto &summedUnweightedGR = GenericListHelper<PartialSet>::realise(dissolve.processingModuleData(), "UnweightedGR",
                                                                      uniqueName_, GenericItem::InRestartFileFlag);

    // Sum the partials from the associated Configurations
    if (!RDFModule::sumUnweightedGR(procPool, this, dissolve.processingModuleData(), summedUnweightedGR))
        return false;

    return true;
}
