// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#include "classes/atomtype.h"
#include "gui/delegates/combolist.hui"
#include "gui/delegates/customcombodelegate.h"
#include "gui/delegates/exponentialspin.hui"
#include "gui/delegates/integerspin.hui"
#include "gui/delegates/intraformcombo.hui"
#include "gui/delegates/isotopecombo.hui"
#include "gui/delegates/null.h"
#include "gui/getspeciesnamedialog.h"
#include "gui/gui.h"
#include "gui/speciestab.h"
#include "main/dissolve.h"
#include <QMessageBox>

SpeciesTab::SpeciesTab(DissolveWindow *dissolveWindow, Dissolve &dissolve, MainTabsWidget *parent, const QString title,
                       Species *species)
    : MainTab(dissolveWindow, dissolve, parent, QString("Species: %1").arg(title), this), atoms_(species->atoms(), dissolve),
      angles_(species->angles(), dissolve), bonds_(species->bonds(), dissolve), torsions_(species->torsions(), dissolve),
      impropers_(species->impropers(), dissolve), isos_(*species), sites_(species->sites())
{
    ui_.setupUi(this);

    Locker refreshLocker(refreshLock_);

    species_ = species;

    // Set item delegates
    // -- SpeciesAtomTable
    ui_.AtomTable->setItemDelegateForColumn(1, new CustomComboDelegate<SpeciesTab>(this, &SpeciesTab::validAtomTypeNames));
    for (auto n = 2; n < 6; ++n)
        ui_.AtomTable->setItemDelegateForColumn(n, new ExponentialSpinDelegate(this));
    // -- Geometry tables
    ui_.BondTable->setItemDelegateForColumn(
        2, new IntraFormComboDelegate(this, new ComboEnumOptionsItems<SpeciesBond::BondFunction>(SpeciesBond::bondFunctions()),
                                      dissolve.masterBonds()));
    ui_.AngleTable->setItemDelegateForColumn(
        3,
        new IntraFormComboDelegate(this, new ComboEnumOptionsItems<SpeciesAngle::AngleFunction>(SpeciesAngle::angleFunctions()),
                                   dissolve.masterAngles()));
    ui_.TorsionTable->setItemDelegateForColumn(
        4, new IntraFormComboDelegate(
               this, new ComboEnumOptionsItems<SpeciesTorsion::TorsionFunction>(SpeciesTorsion::torsionFunctions()),
               dissolve.masterTorsions()));
    ui_.ImproperTable->setItemDelegateForColumn(
        4, new IntraFormComboDelegate(
               this, new ComboEnumOptionsItems<SpeciesTorsion::TorsionFunction>(SpeciesTorsion::torsionFunctions()),
               dissolve.masterImpropers()));
    // -- Isotopologues Tree
    ui_.IsotopologuesTree->setModel(&isos_);
    ui_.IsotopologuesTree->setItemDelegateForColumn(1, new NullDelegate(this));
    ui_.IsotopologuesTree->setItemDelegateForColumn(2, new IsotopeComboDelegate(this));

    // Set models and ensure fonts for table headers are set correctly and the headers themselves are visible
    ui_.AtomTable->setModel(&atoms_);
    ui_.AtomTable->horizontalHeader()->setFont(font());
    ui_.AtomTable->horizontalHeader()->setVisible(true);
    ui_.AtomTable->verticalHeader()->setFont(font());
    ui_.AtomTable->verticalHeader()->setVisible(true);
    ui_.BondTable->setModel(&bonds_);
    ui_.BondTable->horizontalHeader()->setFont(font());
    ui_.BondTable->horizontalHeader()->setVisible(true);
    ui_.AngleTable->setModel(&angles_);
    ui_.AngleTable->horizontalHeader()->setFont(font());
    ui_.AngleTable->horizontalHeader()->setVisible(true);
    ui_.TorsionTable->setModel(&torsions_);
    ui_.TorsionTable->horizontalHeader()->setFont(font());
    ui_.TorsionTable->horizontalHeader()->setVisible(true);
    ui_.ImproperTable->setModel(&impropers_);
    ui_.ImproperTable->horizontalHeader()->setFont(font());
    ui_.ImproperTable->horizontalHeader()->setVisible(true);

    // Set sites model and connect signals
    ui_.SiteList->setModel(&sites_);
    connect(ui_.SiteList->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this,
            SLOT(siteSelectionChanged(const QItemSelection &, const QItemSelection &)));

    // Set up SpeciesViewer
    ui_.ViewerWidget->setDissolve(&dissolve);
    ui_.ViewerWidget->setSpecies(species_);

    // Set up SiteViewer
    ui_.SiteViewerWidget->setCoreData(&dissolve.coreData());
    ui_.SiteViewerWidget->setSpecies(species_);

    // Connect signals / slots
    connect(ui_.ViewerWidget, SIGNAL(dataModified()), this, SLOT(updateControls()));
    connect(ui_.ViewerWidget, SIGNAL(dataModified()), dissolveWindow_, SLOT(setModified()));
    connect(ui_.ViewerWidget->speciesViewer(), SIGNAL(atomsChanged()), this, SLOT(updateAtomTableSelection()));
    connect(ui_.SiteViewerWidget, SIGNAL(dataModified()), this, SLOT(updateSitesTab()));
    connect(ui_.SiteViewerWidget, SIGNAL(siteCreatedAndShown()), this, SLOT(setCurrentSiteFromViewer()));
    connect(ui_.SiteViewerWidget, SIGNAL(dataModified()), dissolveWindow_, SLOT(setModified()));

    connect(&atoms_, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), dissolveWindow_, SLOT(setModified()));
    connect(&atoms_, SIGNAL(atomTypeChanged()), this, SLOT(updateIsotopologuesTab()));
    connect(ui_.AtomTable->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this,
            SLOT(updateUnderlyingAtomSelection()));
    connect(&bonds_, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), dissolveWindow_, SLOT(setModified()));
    connect(&angles_, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), dissolveWindow_, SLOT(setModified()));
    connect(&torsions_, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), dissolveWindow_, SLOT(setModified()));
    connect(&impropers_, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), dissolveWindow_, SLOT(setModified()));
    connect(&isos_, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), dissolveWindow_, SLOT(setModified()));

    connect(ui_.IsotopologuesTree->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(updateIsotopologuesTab()));

    connect(ui_.IsotopologueAddButton, SIGNAL(clicked()), &isos_, SLOT(addIso()));
}

SpeciesTab::~SpeciesTab()
{
    // Remove the Species represented in this tab
    dissolve_.removeSpecies(species_);
}

/*
 * UI
 */

// Update controls in tab
void SpeciesTab::updateControls()
{
    Locker refreshLocker(refreshLock_);

    // View / Generate Tab
    ui_.ViewerWidget->postRedisplay();

    // Geometry Tab
    updateGeometryTab();

    // Isotopologues Tab
    updateIsotopologuesTab();

    // Sites Tab
    updateSitesTab();
}

// Disable sensitive controls within tab
void SpeciesTab::disableSensitiveControls() { setEnabled(false); }

// Enable sensitive controls within tab
void SpeciesTab::enableSensitiveControls() { setEnabled(true); }

/*
 * MainTab Reimplementations
 */

// Return tab type
MainTab::TabType SpeciesTab::type() const { return MainTab::TabType::Species; }

// Raise suitable dialog for entering / checking new tab name
QString SpeciesTab::getNewTitle(bool &ok)
{
    // Get a new, valid name for the Species
    GetSpeciesNameDialog nameDialog(this, dissolve_.coreData());
    ok = nameDialog.get(species_, QString::fromStdString(std::string(species_->name())));

    if (ok)
    {
        // Rename our Species, and flag that our data has been modified
        species_->setName(qPrintable(nameDialog.newName()));

        dissolveWindow_->setModified();
    }

    return nameDialog.newName();
}

// Return whether the title of the tab can be changed
bool SpeciesTab::canChangeTitle() const { return true; }

// Return whether the tab can be closed (after any necessary user querying, etc.)
bool SpeciesTab::canClose() const
{
    // Check that we really want to delete this tab
    QMessageBox queryBox;
    queryBox.setText(QString("Really delete the species '%1'?\nThis cannot be undone!")
                         .arg(QString::fromStdString(std::string(species_->name()))));
    queryBox.setInformativeText("Proceed?");
    queryBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    queryBox.setDefaultButton(QMessageBox::No);
    auto ret = queryBox.exec();

    if (ret != QMessageBox::Yes)
        return false;

    return true;
}

/*
 * Species Target
 */

// Return displayed Species
Species *SpeciesTab::species() const { return species_; }
