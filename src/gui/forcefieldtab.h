// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#pragma once

#include "classes/atomtype.h"
#include "classes/masterintra.h"
#include "classes/pairpotential.h"
#include "gui/maintab.h"
#include "gui/models/atomTypeModel.h"
#include "gui/models/masterTermModel.h"
#include "gui/models/pairPotentialModel.h"
#include "gui/ui_forcefieldtab.h"

Q_DECLARE_METATYPE(const MasterIntra *)
Q_DECLARE_METATYPE(const AtomType *)
Q_DECLARE_METATYPE(const PairPotential *)

// Forcefield Tab
class ForcefieldTab : public QWidget, public MainTab
{
    // All Qt declarations derived from QObject must include this macro
    Q_OBJECT

    public:
    ForcefieldTab(DissolveWindow *dissolveWindow, Dissolve &dissolve, MainTabsWidget *parent, const QString title);
    ~ForcefieldTab() = default;

    /*
     * UI
     */
    private:
    // Main form declaration
    Ui::ForcefieldTab ui_;
    // Models
    AtomTypeModel atoms_;
    PairPotentialModel pairs_;

    /*
     * MainTab Reimplementations
     */
    public:
    // Return tab type
    MainTab::TabType type() const;
    // Return whether the tab can be closed
    bool canClose() const;

    /*
     * Update
     */
    private:
    // Row update function for BondsTable
    void updateBondsTableRow(int row, MasterIntra *masterBond, bool createItems);
    // Row update function for AnglesTable
    void updateAnglesTableRow(int row, MasterIntra *masterAngle, bool createItems);
    // Row update function for TorsionsTable
    void updateTorsionsTableRow(int row, MasterIntra *masterTorsion, bool createItems);
    // Row update function for ImpropersTable
    void updateImpropersTableRow(int row, MasterIntra *masterImproper, bool createItems);
    // Row update function for PairPotentialsTable
    void updatePairPotentialsTableRow(int row, PairPotential *pairPotential, bool createItems);

    // Table models
    MasterTermBondModel *masterBondsTableModel_;
    MasterTermAngleModel *masterAnglesTableModel_;
    MasterTermTorsionModel *masterTorsionsTableModel_;
    MasterTermTorsionModel *masterImpropersTableModel_;

    protected:
    // Update controls in tab
    void updateControls();
    // Disable sensitive controls within tab
    void disableSensitiveControls();
    // Enable sensitive controls within tab
    void enableSensitiveControls();

    /*
     * Signals / Slots
     */
    private:
    // Signal that some AtomType parameter has been modified, so pair potentials should be regenerated
    void atomTypeDataModified();

    private slots:
    // Atom Types
    void on_AtomTypeAddButton_clicked(bool checked);
    void on_AtomTypeRemoveButton_clicked(bool checked);
    // Pair Potentials
    void on_PairPotentialRangeSpin_valueChanged(double value);
    void on_PairPotentialDeltaSpin_valueChanged(double value);
    void on_PairPotentialsIncludeCoulombRadio_clicked(bool checked);
    void on_PairPotentialsShortRangeOnlyRadio_clicked(bool checked);
    void on_ShortRangeTruncationCombo_currentIndexChanged(int index);
    void on_CoulombTruncationCombo_currentIndexChanged(int index);
    void on_RegenerateAllPairPotentialsButton_clicked(bool checked);
    void on_UpdatePairPotentialsButton_clicked(bool checked);
    void on_AutoUpdatePairPotentialsCheck_clicked(bool checked);
    // Master Terms
    void on_MasterTermAddBondButton_clicked(bool checked);
    void on_MasterTermRemoveBondButton_clicked(bool checked);
    void on_MasterTermAddAngleButton_clicked(bool checked);
    void on_MasterTermRemoveAngleButton_clicked(bool checked);
    void on_MasterTermAddTorsionButton_clicked(bool checked);
    void on_MasterTermRemoveTorsionButton_clicked(bool checked);
    void on_MasterTermAddImproperButton_clicked(bool checked);
    void on_MasterTermRemoveImproperButton_clicked(bool checked);
};
