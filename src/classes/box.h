/*
	*** Box Definition
	*** src/classes/box.h
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

#ifndef DISSOLVE_BOX_H
#define DISSOLVE_BOX_H

#include "math/matrix3.h"
#include "base/enumoptions.h"
#include "templates/array.h"

// Forward Declarations
class Atom;
class Cell;
class Data1D;
class ProcessPool;

// Basic Box Definition
class Box
{
	public:
	// Constructor
	Box();
	// Virtual Destructor
	virtual ~Box();
	// Assignment operator
	void operator=(const Box& source);


	/*
	 * Basic Definition
	 */
	public:
	// Box Type Enum
	enum BoxType {
		NonPeriodicBoxType,		/* Non-periodic system - cubic box, but no minimum image calculation */
		CubicBoxType,			/* Cubic box with equivalent cell lengths, and right-angles */
		OrthorhombicBoxType,		/* Orthorhombic box with inequivalent cell lengths, and right-angles */
		MonoclinicBoxType,		/* Monoclinic box with cell angles a != 90, and b == c == 90 */
		TriclinicBoxType,		/* Triclinic box with cell angles a != b != c != 90 */
		nBoxTypes			/* Number of Box types */
	};
	// Return enum options for BoxType
	static EnumOptions<BoxType> boxTypes();

	protected:
	// Box type
	BoxType type_;
	// Box lengths
	double a_, b_, c_;
	// Reciprocal Box lengths
	double ra_, rb_, rc_;
	// Box angles
	double alpha_, beta_, gamma_;
	// Flags stating periodicity along x, y, and z
	Vec3<bool> periodic_;
	// Axes
	Matrix3 axes_;
	// Inverse axes
	Matrix3 inverseAxes_;
	// Reciprocal axes
	Matrix3 reciprocalAxes_;
	// Volume
	double volume_;
	// Reciprocal volume
	double reciprocalVolume_;

	public:
	// Finalise Box, storing volume and reciprocal and inverted axes
	void finalise();
	// Return Box type
	BoxType type() const;
	// Return volume
	double volume() const;
	// Return axis lengths
	Vec3<double> axisLengths() const;
	// Return axis length specified
	double axisLength(int n) const;
	// Return axis angles
	Vec3<double> axisAngles() const;
	// Return axis angle specified
	double axisAngle(int n) const;
	// Return axis matrix
	const Matrix3& axes() const;
	// Return inverse axes matrix
	const Matrix3& inverseAxes() const;
	// Return reciprocal box volume
	double reciprocalVolume() const;
	// Return reciprocal axis lengths
	Vec3<double> reciprocalAxisLengths() const;
	// Return reciprocal axes matrix
	const Matrix3& reciprocalAxes() const;
	// Scale Box by specified factor
	void scale(double factor);


	/*
	 * Minimum Image Routines (Pure Virtual)
	 */
	public:
	// Return minimum image coordinates of 'i' with respect to 'ref'
	virtual Vec3<double> minimumImage(const Atom* i, const Atom* ref) const = 0;
	// Return minimum image coordinates of 'i' with respect to 'ref'
	virtual Vec3<double> minimumImage(const Atom* i, const Vec3<double>& ref) const = 0;
	// Return minimum image coordinates of 'i' with respect to 'ref'
	virtual Vec3<double> minimumImage(const Vec3<double>& i, const Vec3<double>& ref) const = 0;
	// Return minimum image vector from 'i' to 'j'
	virtual Vec3<double> minimumVector(const Atom* i, const Atom* j) const = 0;
	// Return minimum image vector from 'i' to 'j'
	virtual Vec3<double> minimumVector(const Atom& i, const Atom& j) const = 0;
	// Return minimum image vector from 'i' to 'j'
	virtual Vec3<double> minimumVector(const Atom* i, const Vec3<double>& j) const = 0;
	// Return minimum image vector from 'i' to 'j'
	virtual Vec3<double> minimumVector(const Vec3<double>& i, const Vec3<double>& j) const = 0;
	// Return minimum image distance from 'i' to 'j'
	virtual double minimumDistance(const Atom* i, const Atom* j) const = 0;
	// Return minimum image distance from 'i' to 'j' (references)
	virtual double minimumDistance(const Atom& i, const Atom& j) const = 0;
	// Return minimum image distance from 'i' to 'j'
	virtual double minimumDistance(const Atom* i, const Vec3<double>& j) const = 0;
	// Return minimum image distance from 'i' to 'j'
	virtual double minimumDistance(const Vec3<double>& i, const Vec3<double>& j) const = 0;
	// Return minimum image squared distance from 'i' to 'j' (pointers)
	virtual double minimumDistanceSquared(const Atom* i, const Atom* j) const = 0;
	// Return minimum image squared distance from 'i' to 'j' (references)
	virtual double minimumDistanceSquared(const Atom& i, const Atom& j) const = 0;
	// Return minimum image squared distance from 'i' to 'j'
	virtual double minimumDistanceSquared(const Atom* i, const Vec3<double>& j) const = 0;
	// Return minimum image squared distance from 'i' to 'j'
	virtual double minimumDistanceSquared(const Vec3<double>& i, const Vec3<double>& j) const = 0;


	/*
	 * Utility Routines
	 */
	public:
	// Generate a suitable Box given the supplied relative lengths, angles, and volume
	static Box* generate(Vec3<double> lengths, Vec3<double> angles);
	// Return radius of largest possible inscribed sphere for box
	double inscribedSphereRadius() const;
	// Calculate the RDF normalisation for the Box
	bool calculateRDFNormalisation(ProcessPool& procPool, Data1D& boxNorm, double rdfRange, double rdfBinWidth, int nPoints) const;


	/*
	 * Utility Routines (Pure Virtual)
	 */
	public:
	// Return random coordinate inside Box
	virtual Vec3<double> randomCoordinate() const = 0;
	// Return folded coordinate (i.e. inside current Box)
	virtual Vec3<double> fold(const Vec3<double>& i) const = 0;
	// Return folded fractional coordinate (i.e. inside current Box)
	virtual Vec3<double> foldFrac(const Vec3<double>& i) const = 0;
	// Convert supplied fractional coordinates to real space
	virtual Vec3<double> fracToReal(const Vec3<double>& r) const = 0;


	/*
	 * Utility Routines
	 */
	public:
	// Return angle (in degrees, no MIM) between Atoms
	double angleInDegrees(const Atom* i, const Atom* j, const Atom* k) const;
	// Return angle (in degrees) between coordinates
	double angleInDegrees(const Vec3<double>& i, const Vec3<double>& j, const Vec3<double>& k) const;
	// Return angle (in degrees) between supplied normalised vectors
	static double angleInDegrees(const Vec3<double>& normji, const Vec3<double>& normjk);
	// Return angle (in degrees) between supplied normalised vectors (storing dot product)
	static double angleInDegrees(const Vec3<double>& normji, const Vec3<double>& normjk, double& dotProduct);
	// Return literal angle (in degrees) between coordinates, without applying minimum image convention
	static double literalAngleInDegrees(const Vec3<double>& i, const Vec3<double>& j, const Vec3<double>& k);
	// Return torsion (in degrees) between supplied unnormalised vectors
	static double torsionInDegrees(const Vec3<double>& vecji, const Vec3<double>& vecjk, const Vec3<double>& veckl);
	// Return torsion (in degrees) between supplied unnormalised vectors, storing cross products and magnitude in supplied variables
	static double torsionInDegrees(const Vec3<double>& vecji, const Vec3<double>& vecjk, const Vec3<double>& veckl, Vec3<double>& xpj, double& magxpj, Vec3<double>& xpk, double& magxpk);
	// Return torsion (in radians) between supplied unnormalised vectors
	static double torsionInRadians(const Vec3<double>& vecji, const Vec3<double>& vecjk, const Vec3<double>& veckl);
	// Return torsion (in radians) between supplied unnormalised vectors, storing cross products and magnitude in supplied variables
	static double torsionInRadians(const Vec3<double>& vecji, const Vec3<double>& vecjk, const Vec3<double>& veckl, Vec3<double>& xpj, double& magxpj, Vec3<double>& xpk, double& magxpk);
};

// Non-Periodic Box Definition
class NonPeriodicBox : public Box
{
	public:
	// Constructor
	NonPeriodicBox(double length);
	// Destructor
	~NonPeriodicBox();


	/*
	 * Minimum Image Routines (Virtual Implementations)
	 */
	public:
	// Return minimum image coordinates of 'i' with respect to 'ref'
	Vec3<double> minimumImage(const Atom* i, const Atom* ref) const;
	// Return minimum image coordinates of 'i' with respect to 'ref'
	Vec3<double> minimumImage(const Atom* i, const Vec3<double>& ref) const;
	// Return minimum image coordinates of 'i' with respect to 'ref'
	Vec3<double> minimumImage(const Vec3<double>& i, const Vec3<double>& ref) const;
	// Return minimum image vector from 'i' to 'j'
	Vec3<double> minimumVector(const Atom* i, const Atom* j) const;
	// Return minimum image vector from 'i' to 'j'
	Vec3<double> minimumVector(const Atom& i, const Atom& j) const;
	// Return minimum image vector from 'i' to 'j'
	Vec3<double> minimumVector(const Atom* i, const Vec3<double>& j) const;
	// Return minimum image vector from 'i' to 'j'
	Vec3<double> minimumVector(const Vec3<double>& i, const Vec3<double>& j) const;
	// Return minimum image distance from 'i' to 'j'
	double minimumDistance(const Atom* i, const Atom* j) const;
	// Return minimum image distance from 'i' to 'j' (references)
	double minimumDistance(const Atom& i, const Atom& j) const;
	// Return minimum image distance from 'i' to 'j'
	double minimumDistance(const Atom* i, const Vec3<double>& j) const;
	// Return minimum image distance from 'i' to 'j'
	double minimumDistance(const Vec3<double>& i, const Vec3<double>& j) const;
	// Return minimum image squared distance from 'i' to 'j' (pointers)
	double minimumDistanceSquared(const Atom* i, const Atom* j) const;
	// Return minimum image squared distance from 'i' to 'j' (references)
	double minimumDistanceSquared(const Atom& i, const Atom& j) const;
	// Return minimum image squared distance from 'i' to 'j'
	double minimumDistanceSquared(const Atom* i, const Vec3<double>& j) const;
	// Return minimum image squared distance from 'i' to 'j'
	double minimumDistanceSquared(const Vec3<double>& i, const Vec3<double>& j) const;


	/*
	 * Utility Routines (Virtual Implementations)
	 */
	public:
	// Return random coordinate inside Box
	Vec3<double> randomCoordinate() const;
	// Return folded coordinate (i.e. inside current Box)
	Vec3<double> fold(const Vec3<double>& i) const;
	// Return folded fractional coordinate (i.e. inside current Box)
	Vec3<double> foldFrac(const Vec3<double>& i) const;
	// Convert supplied fractional coordinates to real space
	Vec3<double> fracToReal(const Vec3<double>& r) const;
};

// Cubic Box Definition
class CubicBox : public Box
{
	public:
	// Constructor
	CubicBox(double length);
	// Destructor
	~CubicBox();


	/*
	 * Minimum Image Routines (Virtual Implementations)
	 */
	public:
	// Return minimum image coordinates of 'i' with respect to 'ref'
	Vec3<double> minimumImage(const Atom* i, const Atom* ref) const;
	// Return minimum image coordinates of 'i' with respect to 'ref'
	Vec3<double> minimumImage(const Atom* i, const Vec3<double>& ref) const;
	// Return minimum image coordinates of 'i' with respect to 'ref'
	Vec3<double> minimumImage(const Vec3<double>& i, const Vec3<double>& ref) const;
	// Return minimum image vector from 'i' to 'j'
	Vec3<double> minimumVector(const Atom* i, const Atom* j) const;
	// Return minimum image vector from 'i' to 'j'
	Vec3<double> minimumVector(const Atom& i, const Atom& j) const;
	// Return minimum image vector from 'i' to 'j'
	Vec3<double> minimumVector(const Atom* i, const Vec3<double>& j) const;
	// Return minimum image vector from 'i' to 'j'
	Vec3<double> minimumVector(const Vec3<double>& i, const Vec3<double>& j) const;
	// Return minimum image distance from 'i' to 'j'
	double minimumDistance(const Atom* i, const Atom* j) const;
	// Return minimum image distance from 'i' to 'j' (references)
	double minimumDistance(const Atom& i, const Atom& j) const;
	// Return minimum image distance from 'i' to 'j'
	double minimumDistance(const Atom* i, const Vec3<double>& j) const;
	// Return minimum image distance from 'i' to 'j'
	double minimumDistance(const Vec3<double>& i, const Vec3<double>& j) const;
	// Return minimum image squared distance from 'i' to 'j' (pointers)
	double minimumDistanceSquared(const Atom* i, const Atom* j) const;
	// Return minimum image squared distance from 'i' to 'j' (references)
	double minimumDistanceSquared(const Atom& i, const Atom& j) const;
	// Return minimum image squared distance from 'i' to 'j'
	double minimumDistanceSquared(const Atom* i, const Vec3<double>& j) const;
	// Return minimum image squared distance from 'i' to 'j'
	double minimumDistanceSquared(const Vec3<double>& i, const Vec3<double>& j) const;


	/*
	 * Utility Routines (Virtual Implementations)
	 */
	public:
	// Return random coordinate inside Box
	Vec3<double> randomCoordinate() const;
	// Return folded coordinate (i.e. inside current Box)
	Vec3<double> fold(const Vec3<double>& i) const;
	// Return folded fractional coordinate (i.e. inside current Box)
	Vec3<double> foldFrac(const Vec3<double>& i) const;
	// Convert supplied fractional coordinates to real space
	Vec3<double> fracToReal(const Vec3<double>& r) const;
};

// Orthorhombic Box Definition
class OrthorhombicBox : public Box
{
	public:
	// Constructor
	OrthorhombicBox(const Vec3<double> lengths);
	// Destructor
	~OrthorhombicBox();


	/*
	 * Minimum Image Routines (Virtual Implementations)
	 */
	public:
	// Return minimum image coordinates of 'i' with respect to 'ref'
	Vec3<double> minimumImage(const Atom* i, const Atom* ref) const;
	// Return minimum image coordinates of 'i' with respect to 'ref'
	Vec3<double> minimumImage(const Atom* i, const Vec3<double>& ref) const;
	// Return minimum image coordinates of 'i' with respect to 'ref'
	Vec3<double> minimumImage(const Vec3<double>& i, const Vec3<double>& ref) const;
	// Return minimum image vector from 'i' to 'j'
	Vec3<double> minimumVector(const Atom* i, const Atom* j) const;
	// Return minimum image vector from 'i' to 'j'
	Vec3<double> minimumVector(const Atom& i, const Atom& j) const;
	// Return minimum image vector from 'i' to 'j'
	Vec3<double> minimumVector(const Atom* i, const Vec3<double>& j) const;
	// Return minimum image vector from 'i' to 'j'
	Vec3<double> minimumVector(const Vec3<double>& i, const Vec3<double>& j) const;
	// Return minimum image distance from 'i' to 'j'
	double minimumDistance(const Atom* i, const Atom* j) const;
	// Return minimum image distance from 'i' to 'j' (references)
	double minimumDistance(const Atom& i, const Atom& j) const;
	// Return minimum image distance from 'i' to 'j'
	double minimumDistance(const Atom* i, const Vec3<double>& j) const;
	// Return minimum image distance from 'i' to 'j'
	double minimumDistance(const Vec3<double>& i, const Vec3<double>& j) const;
	// Return minimum image squared distance from 'i' to 'j' (pointers)
	double minimumDistanceSquared(const Atom* i, const Atom* j) const;
	// Return minimum image squared distance from 'i' to 'j' (references)
	double minimumDistanceSquared(const Atom& i, const Atom& j) const;
	// Return minimum image squared distance from 'i' to 'j'
	double minimumDistanceSquared(const Atom* i, const Vec3<double>& j) const;
	// Return minimum image squared distance from 'i' to 'j'
	double minimumDistanceSquared(const Vec3<double>& i, const Vec3<double>& j) const;


	/*
	 * Utility Routines (Virtual Implementations)
	 */
	public:
	// Return random coordinate inside Box
	Vec3<double> randomCoordinate() const;
	// Return folded coordinate (i.e. inside current Box)
	Vec3<double> fold(const Vec3<double>& i) const;
	// Return folded fractional coordinate (i.e. inside current Box)
	Vec3<double> foldFrac(const Vec3<double>& i) const;
	// Convert supplied fractional coordinates to real space
	Vec3<double> fracToReal(const Vec3<double>& r) const;
};

// Monoclinic Box Definition
class MonoclinicBox : public Box
{
	public:
	// Constructor
	MonoclinicBox(const Vec3<double> lengths, double beta);
	// Destructor
	~MonoclinicBox();


	/*
	 * Minimum Image Routines (Virtual Implementations)
	 */
	public:
	// Return minimum image coordinates of 'i' with respect to 'ref'
	Vec3<double> minimumImage(const Atom* i, const Atom* ref) const;
	// Return minimum image coordinates of 'i' with respect to 'ref'
	Vec3<double> minimumImage(const Atom* i, const Vec3<double>& ref) const;
	// Return minimum image coordinates of 'i' with respect to 'ref'
	Vec3<double> minimumImage(const Vec3<double>& i, const Vec3<double>& ref) const;
	// Return minimum image vector from 'i' to 'j'
	Vec3<double> minimumVector(const Atom* i, const Atom* j) const;
	// Return minimum image vector from 'i' to 'j'
	Vec3<double> minimumVector(const Atom& i, const Atom& j) const;
	// Return minimum image vector from 'i' to 'j'
	Vec3<double> minimumVector(const Atom* i, const Vec3<double>& j) const;
	// Return minimum image vector from 'i' to 'j'
	Vec3<double> minimumVector(const Vec3<double>& i, const Vec3<double>& j) const;
	// Return minimum image distance from 'i' to 'j'
	double minimumDistance(const Atom* i, const Atom* j) const;
	// Return minimum image distance from 'i' to 'j' (references)
	double minimumDistance(const Atom& i, const Atom& j) const;
	// Return minimum image distance from 'i' to 'j'
	double minimumDistance(const Atom* i, const Vec3<double>& j) const;
	// Return minimum image distance from 'i' to 'j'
	double minimumDistance(const Vec3<double>& i, const Vec3<double>& j) const;
	// Return minimum image squared distance from 'i' to 'j' (pointers)
	double minimumDistanceSquared(const Atom* i, const Atom* j) const;
	// Return minimum image squared distance from 'i' to 'j' (references)
	double minimumDistanceSquared(const Atom& i, const Atom& j) const;
	// Return minimum image squared distance from 'i' to 'j'
	double minimumDistanceSquared(const Atom* i, const Vec3<double>& j) const;
	// Return minimum image squared distance from 'i' to 'j'
	double minimumDistanceSquared(const Vec3<double>& i, const Vec3<double>& j) const;


	/*
	 * Utility Routines (Virtual Implementations)
	 */
	public:
	// Return random coordinate inside Box
	Vec3<double> randomCoordinate() const;
	// Return folded coordinate (i.e. inside current Box)
	Vec3<double> fold(const Vec3<double>& i) const;
	// Return folded fractional coordinate (i.e. inside current Box)
	Vec3<double> foldFrac(const Vec3<double>& i) const;
	// Convert supplied fractional coordinates to real space
	Vec3<double> fracToReal(const Vec3<double>& r) const;
};

// Triclinic Box Definition
class TriclinicBox : public Box
{
	public:
	// Constructor
	TriclinicBox(const Vec3<double> lengths, const Vec3<double> angles);
	// Destructor
	~TriclinicBox();


	/*
	 * Minimum Image Routines (Virtual Implementations)
	 */
	public:
	// Return minimum image coordinates of 'i' with respect to 'ref'
	Vec3<double> minimumImage(const Atom* i, const Atom* ref) const;
	// Return minimum image coordinates of 'i' with respect to 'ref'
	Vec3<double> minimumImage(const Atom* i, const Vec3<double>& ref) const;
	// Return minimum image coordinates of 'i' with respect to 'ref'
	Vec3<double> minimumImage(const Vec3<double>& i, const Vec3<double>& ref) const;
	// Return minimum image vector from 'i' to 'j'
	Vec3<double> minimumVector(const Atom* i, const Atom* j) const;
	// Return minimum image vector from 'i' to 'j'
	Vec3<double> minimumVector(const Atom& i, const Atom& j) const;
	// Return minimum image vector from 'i' to 'j'
	Vec3<double> minimumVector(const Atom* i, const Vec3<double>& j) const;
	// Return minimum image vector from 'i' to 'j'
	Vec3<double> minimumVector(const Vec3<double>& i, const Vec3<double>& j) const;
	// Return minimum image distance from 'i' to 'j'
	double minimumDistance(const Atom* i, const Atom* j) const;
	// Return minimum image distance from 'i' to 'j' (references)
	double minimumDistance(const Atom& i, const Atom& j) const;
	// Return minimum image distance from 'i' to 'j'
	double minimumDistance(const Atom* i, const Vec3<double>& j) const;
	// Return minimum image distance from 'i' to 'j'
	double minimumDistance(const Vec3<double>& i, const Vec3<double>& j) const;
	// Return minimum image squared distance from 'i' to 'j' (pointers)
	double minimumDistanceSquared(const Atom* i, const Atom* j) const;
	// Return minimum image squared distance from 'i' to 'j' (references)
	double minimumDistanceSquared(const Atom& i, const Atom& j) const;
	// Return minimum image squared distance from 'i' to 'j'
	double minimumDistanceSquared(const Atom* i, const Vec3<double>& j) const;
	// Return minimum image squared distance from 'i' to 'j'
	double minimumDistanceSquared(const Vec3<double>& i, const Vec3<double>& j) const;


	/*
	 * Utility Routines (Virtual Implementations)
	 */
	public:
	// Return random coordinate inside Box
	Vec3<double> randomCoordinate() const;
	// Return folded coordinate (i.e. inside current Box)
	Vec3<double> fold(const Vec3<double>& i) const;
	// Return folded fractional coordinate (i.e. inside current Box)
	Vec3<double> foldFrac(const Vec3<double>& i) const;
	// Convert supplied fractional coordinates to real space
	Vec3<double> fracToReal(const Vec3<double>& r) const;
};

#endif
