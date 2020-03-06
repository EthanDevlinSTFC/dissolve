/*
	*** Export Module - Processing
	*** src/modules/export/process.cpp
	Copyright T. Youngs 2012-2020

	This file is part of Dissolve.

	Dissolve is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Dissolve is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Dissolve.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "modules/export/export.h"
#include "main/dissolve.h"
#include "classes/atom.h"
#include "classes/atomtype.h"
#include "classes/box.h"
#include "base/sysfunc.h"
#include "base/lineparser.h"

// Run main processing
bool ExportModule::process(Dissolve& dissolve, ProcessPool& procPool)
{
	/*
	 * Export data
	 */

	/*
	 * Write coordinates
	 */

	if (coordinatesFormat_.hasValidFileAndFormat())
	{
		// Check for zero Configuration targets
		if (targetConfigurations_.nItems() == 0) return Messenger::error("No configuration targets set for module '%s'.\n", uniqueName());
		else
		{
			// Loop over target Configurations
			for (RefListItem<Configuration>* ri = targetConfigurations_.first(); ri != NULL; ri = ri->next())
			{
				// Grab Configuration pointer
				Configuration* cfg = ri->item();

				// Set up process pool - must do this to ensure we are using all available processes
				procPool.assignProcessesToGroups(cfg->processPool());

				// Only the pool master saves the data
				if (procPool.isMaster())
				{
					Messenger::print("Export: Writing coordinates file (%s) for Configuration '%s'...\n", coordinatesFormat_.description(), cfg->name());

					if (!coordinatesFormat_.exportData(cfg))
					{
						Messenger::print("Export: Failed to export coordinates file '%s'.\n", coordinatesFormat_.filename());
						procPool.decideFalse();
						return false;
					}

					procPool.decideTrue();
				}
				else if (!procPool.decision()) return false;
			}
		}
	}

	/*
	 * Write PairPotentials
	 */

	if (pairPotentialFormat_.hasValidFileAndFormat())
	{
		// Only the pool master saves the data
		if (procPool.isMaster())
		{
			// Store the current (root) pair potential filename
			CharString rootPPName = pairPotentialFormat_.filename();

			for (PairPotential* pp = dissolve.pairPotentials().first(); pp != NULL; pp = pp->next())
			{
				Messenger::print("Export: Writing pair potential file (%s) for %s-%s...\n", pairPotentialFormat_.description(), pp->atomTypeNameI(), pp->atomTypeNameJ());

				// Generate filename
				pairPotentialFormat_.setFilename(CharString("%s-%s-%s.pp", rootPPName.get(), pp->atomTypeNameI(), pp->atomTypeNameJ()));

				// Append pair potential
				if (!pairPotentialFormat_.exportData(pp))
				{
					Messenger::print("Export: Failed to export pair potential file '%s'.\n", pairPotentialFormat_.filename());
					pairPotentialFormat_.setFilename(rootPPName);
					procPool.decideFalse();
					return false;
				}

				procPool.decideTrue();
			}

			// Revert root name in FileAndFormat
			pairPotentialFormat_.setFilename(rootPPName);
		}
		else if (!procPool.decision()) return false; 
	}

	/*
	 * Write trajectories
	 */

	if (trajectoryFormat_.hasValidFileAndFormat())
	{
		// Check for zero Configuration targets
		if (targetConfigurations_.nItems() == 0) return Messenger::error("No configuration targets set for module '%s'.\n", uniqueName());
		else
		{
			// Loop over target Configurations
			for (RefListItem<Configuration>* ri = targetConfigurations_.first(); ri != NULL; ri = ri->next())
			{
				// Grab Configuration pointer
				Configuration* cfg = ri->item();

				// Set up process pool - must do this to ensure we are using all available processes
				procPool.assignProcessesToGroups(cfg->processPool());

				// Only the pool master saves the data
				if (procPool.isMaster())
				{
					Messenger::print("Export: Appending trajectory file (%s) for Configuration '%s'...\n", trajectoryFormat_.description(), cfg->name());

					if (!trajectoryFormat_.exportData(cfg))
					{
						Messenger::print("Export: Failed to append trajectory file '%s'.\n", trajectoryFormat_.filename());
						procPool.decideFalse();
						return false;
					}

					procPool.decideTrue();
				}
				else if (!procPool.decision()) return false;
			}
		}
	}

	return true;
}
