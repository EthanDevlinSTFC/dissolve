/*
	*** Module Widget
	*** src/gui/modulewidget.cpp
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

#include "gui/modulewidget.h"

// Constructor
ModuleWidget::ModuleWidget(QWidget* parent) : QWidget(parent)
{
	refreshing_ = false;
}

ModuleWidget::~ModuleWidget()
{
}

/*
 * UI
 */

// Update controls within widget
void ModuleWidget::updateControls(int flags)
{
}

// Disable sensitive controls within widget
void ModuleWidget::disableSensitiveControls()
{
}

// Enable sensitive controls within widget
void ModuleWidget::enableSensitiveControls()
{
}

/*
 * State I/O
 */

// Write widget state through specified LineParser
bool ModuleWidget::writeState(LineParser& parser) const
{
	return true;
}

// Read widget state through specified LineParser
bool ModuleWidget::readState(LineParser& parser)
{
	return true;
}
