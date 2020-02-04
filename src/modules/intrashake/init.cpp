/*
	*** IntraShake Module - Initialisation
	*** src/modules/intrashake/init.cpp
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

#include "modules/intrashake/intrashake.h"
#include "keywords/types.h"

// Perform any necessary initialisation for the Module
void IntraShakeModule::initialise()
{
	// Calculation
	keywords_.add("Calculation", new DoubleKeyword(-1.0), "CutoffDistance", "Interatomic cutoff distance to employ", "<distance>");
	keywords_.add("Calculation", new IntegerKeyword(1), "ShakesPerTerm", "Number of shakes per term", "<n>");
	keywords_.add("Calculation", new DoubleKeyword(0.33), "TargetAcceptanceRate", "Target acceptance rate for Monte Carlo moves", "<rate (0.0-1.0)>");
	keywords_.add("Calculation", new BoolKeyword(false), "TermEnergyOnly", "Whether only the energy of the intramolecular term is calculated and assessed");

	// Bonds
	keywords_.add("Bonds", new BoolKeyword(true), "AdjustBonds", "Whether Bonds in the molecule should be shaken", "<true|false>");
	keywords_.add("Bonds", new DoubleKeyword(0.01), "BondStepSize", "Step size for Bond adjustments (Angstroms)", "<stepsize>", KeywordBase::InRestartFileOption);
	keywords_.add("Bonds", new DoubleKeyword(0.001), "BondStepSizeMin", "Minimum step size for Bond adjustments (Angstroms)", "<stepsize>");
	keywords_.add("Bonds", new DoubleKeyword(0.2), "BondStepSizeMax", "Maximum step size for Bond adjustments (Angstroms)", "<stepsize>");

	// Angles
	keywords_.add("Angles", new BoolKeyword(true), "AdjustAngles", "Whether Angles in the molecule should be shaken", "<true|false>");
	keywords_.add("Angles", new DoubleKeyword(5.0), "AngleStepSize", "Step size for Angle adjustments (degrees)", "<stepsize>", KeywordBase::InRestartFileOption);
	keywords_.add("Angles", new DoubleKeyword(0.01), "AngleStepSizeMin", "Minimum step size for Angle adjustments (degrees)", "<stepsize>");
	keywords_.add("Angles", new DoubleKeyword(20.0), "AngleStepSizeMax", "Maximum step size for Angle adjustments (degrees)", "<stepsize>");

	// Torsions
	keywords_.add("Torsions", new BoolKeyword(true), "AdjustTorsions", "Whether Torsions in the molecule should be shaken", "<true|false>");
	keywords_.add("Torsions", new DoubleKeyword(10.0), "TorsionStepSize", "Step size for Torsion adjustments (degrees)", "<stepsize>", KeywordBase::InRestartFileOption);
	keywords_.add("Torsions", new DoubleKeyword(0.5), "TorsionStepSizeMin", "Minimum step size for Torsion adjustments (degrees)", "<stepsize>");
	keywords_.add("Torsions", new DoubleKeyword(45.0), "TorsionStepSizeMax", "Maximum step size for Torsion adjustments (degrees)", "<stepsize>");
}

