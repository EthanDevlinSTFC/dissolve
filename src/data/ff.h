/*
	*** Forcefield Data
	*** src/data/ff.h
	Copyright T. Youngs 2019-2020

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

#ifndef DISSOLVE_FORCEFIELD_H
#define DISSOLVE_FORCEFIELD_H

#include "data/elements.h"
#include "classes/speciesangle.h"
#include "classes/speciesbond.h"
#include "classes/speciesimproper.h"
#include "classes/speciestorsion.h"
#include "base/enumoptions.h"
#include "templates/reflist.h"

// Forward Declarations
class CoreData;
class ForcefieldAngleTerm;
class ForcefieldAtomType;
class ForcefieldBondTerm;
class ForcefieldImproperTerm;
class ForcefieldParameters;
class ForcefieldTorsionTerm;
class Species;
class SpeciesAtom;

// Forcefield Base Class
class Forcefield : public Elements, public ListItem<Forcefield>
{
	public:
	// Constructor / Destructor
	Forcefield();
	virtual ~Forcefield();


	/*
	 * Definition
	 */
	public:
	// ShortRange Interaction Type
	enum ShortRangeType
	{
		UndefinedType,			/* Undefined short-range type */
		NoInteractionType,		/* No short-range dispersive forces */
		LennardJonesType,		/* Lennard-Jones 12-6 form with Lorentz-Berthelot combination rules */
		LennardJonesGeometricType,	/* Lennard-Jones 12-6 form with Geometric combination rules */
		nShortRangeTypes		/* Number of short-range interaction types */
	};
	// Return enum options for ShortRangeType
	static EnumOptions<ShortRangeType> shortRangeTypes();
	// Return name of Forcefield
	virtual const char* name() const = 0;
	// Return description of Forcefield
	virtual const char* description() const = 0;
	// Return short-range interaction style for AtomTypes
	virtual ShortRangeType shortRangeType() const = 0;


	/*
	 * Atom Type Data
	 */
	protected:
	// Short-range parameter sets
	List<ForcefieldParameters> shortRangeParameters_;
	// Atom type data
	List<ForcefieldAtomType> atomTypes_;
	// Atom type data, grouped by element
	Array< RefList<ForcefieldAtomType> > atomTypesByElementPrivate_;

	protected:
	// Add short-range parameters
	void addParameters(const char* name, double data0, double data1 = 0.0, double data2 = 0.0, double data3 = 0.0);
	// Add new atom type with its own parameters
	void addAtomType(int Z, int index, const char* name, const char* netaDefinition, const char* description, double q, double data0, double data1, double data2 = 0.0, double data3 = 0.0);
	// Add new atom type referencing existing parameters by name
	void addAtomType(int Z, int index, const char* name, const char* netaDefinition, const char* description, double q, const char* parameterReference);
	// Copy existing atom type
	void copyAtomType(const ForcefieldAtomType& sourceType, const char* newTypeName, const char* netaDefinition = NULL, const char* equivalentName = NULL);
	// Determine and return atom type for specified SpeciesAtom from supplied Array of types
	static ForcefieldAtomType* determineAtomType(SpeciesAtom* i, const Array< RefList<ForcefieldAtomType> >& atomTypes);
	// Determine and return atom type for specified SpeciesAtom
	virtual ForcefieldAtomType* determineAtomType(SpeciesAtom* i) const;

	public:
	// Return named short-range parameters (if they exist)
	ForcefieldParameters* shortRangeParameters(const char* name) const;
	// Return the named ForcefieldAtomType (if it exists)
	virtual ForcefieldAtomType* atomTypeByName(const char* name, Element* element = NULL) const;
	// Return the ForcefieldAtomType with specified id (if it exists)
	virtual ForcefieldAtomType* atomTypeById(int id, Element* element = NULL) const;


	/*
	 * Term Data
	 */
	private:
	// Bond terms of the Forcefield
	List<ForcefieldBondTerm> bondTerms_;
	// Angle terms of the Forcefield
	List<ForcefieldAngleTerm> angleTerms_;
	// Torsion terms of the Forcefield
	List<ForcefieldTorsionTerm> torsionTerms_;
	// Improper terms of the Forcefield
	List<ForcefieldImproperTerm> improperTerms_;

	protected:
	// Add bond term
	void addBondTerm(const char* typeI, const char* typeJ, SpeciesBond::BondFunction form, double data0 = 0.0, double data1 = 0.0, double data2 = 0.0, double data3 = 0.0);
	// Add angle term
	void addAngleTerm(const char* typeI, const char* typeJ, const char* typeK, SpeciesAngle::AngleFunction form, double data0 = 0.0, double data1 = 0.0, double data2 = 0.0, double data3 = 0.0);
	// Add torsion term
	void addTorsionTerm(const char* typeI, const char* typeJ, const char* typeK, const char* typeL, SpeciesTorsion::TorsionFunction form, double data0 = 0.0, double data1 = 0.0, double data2 = 0.0, double data3 = 0.0);
	// Add improper term
	void addImproperTerm(const char* typeI, const char* typeJ, const char* typeK, const char* typeL, SpeciesImproper::ImproperFunction form, double data0 = 0.0, double data1 = 0.0, double data2 = 0.0, double data3 = 0.0);

	public:
	// Return bond term for the supplied atom type pair (if it exists)
	virtual ForcefieldBondTerm* bondTerm(const ForcefieldAtomType* i, const ForcefieldAtomType* j) const;
	// Return angle term for the supplied atom type trio (if it exists)
	virtual ForcefieldAngleTerm* angleTerm(const ForcefieldAtomType* i, const ForcefieldAtomType* j, const ForcefieldAtomType* k) const;
	// Return torsion term for the supplied atom type quartet (if it exists)
	virtual ForcefieldTorsionTerm* torsionTerm(const ForcefieldAtomType* i, const ForcefieldAtomType* j, const ForcefieldAtomType* k, const ForcefieldAtomType* l) const;
	// Return improper term for the supplied atom type quartet (if it exists)
	virtual ForcefieldImproperTerm* improperTerm(const ForcefieldAtomType* i, const ForcefieldAtomType* j, const ForcefieldAtomType* k, const ForcefieldAtomType* l) const;


	/*
	 * Term Assignment
	 */
	protected:
	// Assign suitable AtomType to the supplied atom
	virtual bool assignAtomType(SpeciesAtom* i, CoreData& coreData) const;

	public:
	// AtomType Assignment Strategy
	enum AtomTypeAssignmentStrategy
	{
		 TypeAll,		/* Assign atom types to all atoms, overwriting any that exist */
		 TypeMissing,		/* Assign atom types to all atoms that do not currently have a type assigned */
		 TypeSelection		/* Assign atom types to the current selection, overwriting any types on the atoms that already exist */
	};
	// Intramolecular Term Assignment Flags
	enum IntramolecularTermAssignmentFlags
	{
		DetermineTypesFlag = 1,		/* Attempt to determine atom types on-the-fly, rather than use those existing on the atoms */
		GenerateImpropersFlag = 2,	/* Generate improper terms where available */
		SelectionOnlyFlag = 4		/* Only assign terms where all atoms are in the current selection */
	};
	// Assign suitable AtomTypes to the supplied Species, returning the number of failures
	int assignAtomTypes(Species* sp, CoreData& coreData, AtomTypeAssignmentStrategy strategy) const;
	// Assign intramolecular parameters to the supplied Species
	virtual bool assignIntramolecular(Species* sp, int flags = 0) const;


	/*
	 * Atom Environment Helpers
	 */
	public:
	// Atom Geometry enum
	enum AtomGeometry { UnboundGeometry, TerminalGeometry, LinearGeometry, TShapeGeometry, TrigonalPlanarGeometry, TetrahedralGeometry, SquarePlanarGeometry, TrigonalBipyramidalGeometry, OctahedralGeometry, nAtomGeometries };

	protected:
	// Calculate and return the geometry of the specified SpeciesAtom
	AtomGeometry geometryOfAtom(SpeciesAtom* i) const;
	// Return whether the specified SpeciesAtom exists in the specified geometry
	bool isAtomGeometry(SpeciesAtom* i, AtomGeometry geom) const;
	// Return whether supplied bonding pattern around the SpeciesAtom matches *exactly*
	bool isBondPattern(const SpeciesAtom* i, const int nSingle, const int nDouble = 0, const int nTriple = 0, const int nQuadruple = 0, const int nAromatic = 0) const;
	// Return whether the specified atom is bound to a specific element (and count thereof)
	bool isBoundTo(const SpeciesAtom* i, Element* element, const int count = 1, bool allowMoreThanCount = true) const;
	// Guess and return oxidation state for the specified SpeciesAtom
	int guessOxidationState(const SpeciesAtom* i) const;
};

#endif
