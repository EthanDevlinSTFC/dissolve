/*
	*** Cell Distributor
	*** src/classes/celldistributor.cpp
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

#include "classes/celldistributor.h"
#include "classes/cell.h"
#include "base/processpool.h"

// Constructor
CellDistributor::CellDistributor(const CellArray& cellArray, ProcessPool& procPool, ProcessPool::DivisionStrategy strategy, bool repeatsAllowed) : Distributor(cellArray.nCells(), cellArray, procPool, strategy, repeatsAllowed), cells_(cellArray)
{
}

// Destructor
CellDistributor::~CellDistributor()
{
}

/*
 * Cells
 */

// Return array of Cells that we must hard lock in order to modify the object with index specified
Array<Cell*> CellDistributor::cellsToBeModifiedForObject(int objectId)
{
	Array<Cell*> cells;

	cells.add(cells_.cell(objectId));

	return cells;
}
