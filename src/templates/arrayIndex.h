// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#include "base/messenger.h"
#include "templates/list.h"
#include "templates/vector3.h"
#include "templates/array3d.h"
#include <numeric>
#include <tuple>
#include <vector>


template <class A> class ArrayIndex
{
    public:
    ArrayIndex(Array3D& inArray)
    {
        nX = inArray.nX();
        nY = inArray.nY();
        nZ = inArray.nZ();
        arrayIndex = {0,0,0};
        std::vector<A> 
    }
    ArrayIndex(Array2D& inArray)
    {
        nX=inArray.nRows();
        nY=inArray.nColumns();
        nZ=0;
    }

    private:
        int nX;nY;nZ;
        std::tuple<int,int,int> arrayIndex;
}