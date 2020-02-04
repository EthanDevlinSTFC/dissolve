/*
	*** Empirical Formula Generation
	*** src/classes/empiricalformula.h
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

#ifndef DISSOLVE_EMPIRICALFORMULA_H
#define DISSOLVE_EMPIRICALFORMULA_H

#include "base/charstring.h"
#include "templates/array.h"
#include "templates/reflist.h"

// Forward Declarations
class Element;
class Species;
class SpeciesAtom;

// Empirical Formula Generator
class EmpiricalFormula
{
	public:
	// Constructor
	EmpiricalFormula();
	// Destructor
	~EmpiricalFormula();


	/*
	 * Construction
	 */
	private:
	// Element counts
	Array<int> elementCounts_;
	// Formula string
	CharString formula_;

	public:
	// Clear counts array
	void reset();
	// Add to empirical formula
	void add(Element* element, int count = 1);
	// Return current empirical formula
	const char* formula();
	// Return rich text of current empirical formula
	const char* richTextFormula();


	/*
	 * Convenience Functions
	 */
	public:
	// Return empirical formula for supplied Species
	static const char* formula(const Species* species, bool richText = false);
	// Return empirical formula for supplied SpeciesAtom reflist
	static const char* formula(const RefList<SpeciesAtom>& atoms, bool richText = false);
};

#endif
