/*
	*** MolShake Module - Initialisation
	*** src/modules/molshake/init.cpp
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

#include "modules/molshake/molshake.h"
#include "keywords/types.h"

// Perform any necessary initialisation for the Module
void MolShakeModule::initialise()
{
	// Calculation
	keywords_.add("Calculation", new DoubleKeyword(-1.0), "CutoffDistance", "Interatomic cutoff distance to employ", "<rcut>");
	keywords_.add("Calculation", new IntegerKeyword(1), "ShakesPerMolecule", "Number of shakes per Molecule", "<n>");
	keywords_.add("Calculation", new DoubleKeyword(0.33), "TargetAcceptanceRate", "Target acceptance rate for Monte Carlo moves", "<rate (0.0 - 1.0)>");

	// Rotations
	keywords_.add("Rotations", new DoubleKeyword(1.0), "RotationStepSize", "Step size for rotational component of Monte Carlo move (degrees)", "<stepsize>", KeywordBase::InRestartFileOption);
	keywords_.add("Rotations", new DoubleKeyword(0.01), "RotationStepSizeMin", "Minimum step size for rotations (degrees)", "<stepsize>");
	keywords_.add("Rotations", new DoubleKeyword(90.0), "RotationStepSizeMax", "Maximum step size for rotations (degrees)", "<stepsize>");

	// Translations
	keywords_.add("Translations", new DoubleKeyword(0.05), "TranslationStepSize", "Step size for translational component of Monte Carlo move (Angstroms)", "<stepsize>", KeywordBase::InRestartFileOption);
	keywords_.add("Translations", new DoubleKeyword(0.001), "TranslationStepSizeMin", "Minimum step size for translations (Angstroms)", "<stepsize>");
	keywords_.add("Translations", new DoubleKeyword(3.0), "TranslationStepSizeMax", "Maximum step size for translations (Angstroms)", "<stepsize>");
}

