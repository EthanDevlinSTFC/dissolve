// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#pragma once

#include "procedure/nodes/regionbase.h"

// Cylindrical Region
class CylindricalRegionProcedureNode : public RegionProcedureNodeBase
{
    public:
    CylindricalRegionProcedureNode();
    ~CylindricalRegionProcedureNode() override = default;

    /*
     * Control
     */
    private:
    // Cylinder definition variables (retrieved from keywords)
    double r_, denominator_;
    Vec3<double> l0_, l1_;

    /*
     * Region Data
     */
    public:
    // Return whether voxel centred at supplied real coordinates is valid
    bool isVoxelValid(const Configuration *cfg, const Vec3<double> &r) const;

    /*
     * Execute
     */
    public:
    // Prepare any necessary data, ready for execution
    bool prepare(Configuration *cfg, std::string_view prefix, GenericList &targetList) override;
};
