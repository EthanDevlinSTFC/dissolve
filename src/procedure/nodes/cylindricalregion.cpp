// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#include "procedure/nodes/cylindricalregion.h"
#include "classes/configuration.h"
#include "keywords/double.h"
#include "keywords/vec3double.h"

CylindricalRegionProcedureNode::CylindricalRegionProcedureNode()
    : RegionProcedureNodeBase(ProcedureNode::NodeType::CylindricalRegion)
{
    keywords_.add("Control", new Vec3DoubleKeyword({0.0, 0.0, 0.0}, Vec3Labels::XYZLabels), "OriginFrac",
                  "Origin of vector in fractional coordinates");
    keywords_.add("Control", new DoubleKeyword(5.0, 1.0e-3), "Radius", "Cylinder radius");
    keywords_.add("Control", new Vec3DoubleKeyword({0.0, 0.0, 1.0}, Vec3Labels::XYZLabels), "Vector", "Cylinder vector");
}

/*
 * Region Data
 */

// Return whether voxel centred at supplied real coordinates is valid
bool CylindricalRegionProcedureNode::isVoxelValid(const Configuration *cfg, const Vec3<double> &r) const
{
    auto p0 = cfg->box()->minimumImage(r, l0_);
    auto num = ((p0 - l0_) * (p0 - l1_)).magnitude();

    // Check distance vs cylinder radius
    fmt::print("Dist = {}\n", num / denominator_);
    return (num / denominator_) <= r_;
}

/*
 * Execute
 */

// Prepare any necessary data, ready for execution
bool CylindricalRegionProcedureNode::prepare(Configuration *cfg, std::string_view prefix, GenericList &targetList)
{
    // Retrieve keyword values
    l0_ = cfg->box()->axes() * keywords_.asVec3Double("OriginFrac");
    l1_ = l0_ + keywords_.asVec3Double("Vector");
    denominator_ = (l1_ - l0_).magnitude();
    r_ = keywords_.asDouble("Radius");

    return true;
}
