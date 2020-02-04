/*
	*** Calculate_AvgMol Module Widget - Functions
	*** src/modules/calculate_avgmol/gui/modulewidget_funcs.cpp
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

#include "modules/calculate_avgmol/gui/modulewidget.h"
#include "modules/calculate_avgmol/avgmol.h"

// Constructor
CalculateAvgMolModuleWidget::CalculateAvgMolModuleWidget(QWidget* parent, CalculateAvgMolModule* module) : ModuleWidget(parent), module_(module)
{
	// Set up user interface
	ui_.setupUi(this);

	ui_.SpeciesView->setSpecies(&module_->averageSpecies());

	refreshing_ = false;
}

/*
 * UI
 */

// Update controls within widget
void CalculateAvgMolModuleWidget::updateControls(int flags)
{
}

// Disable sensitive controls within widget
void CalculateAvgMolModuleWidget::disableSensitiveControls()
{
}

// Enable sensitive controls within widget
void CalculateAvgMolModuleWidget::enableSensitiveControls()
{
}

/*
 * State I/O
 */

// Write widget state through specified LineParser
bool CalculateAvgMolModuleWidget::writeState(LineParser& parser) const
{
	return true;
}

// Read widget state through specified LineParser
bool CalculateAvgMolModuleWidget::readState(LineParser& parser)
{
	return true;
}

/*
 * Widgets / Functions
 */

