// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#pragma once

#include "procedure/nodes/pickbase.h"

// Forward Declarations
class RegionProcedureNodeBase;

// Pick Region Node
class PickRegionProcedureNode : public PickProcedureNodeBase
{
    public:
    explicit PickRegionProcedureNode(const RegionProcedureNodeBase *region = nullptr);
    ~PickRegionProcedureNode() override = default;

    /*
     * Execute
     */
    public:
    // Execute node, targetting the supplied Configuration
    bool execute(ProcessPool &procPool, Configuration *cfg, std::string_view prefix, GenericList &targetList) override;
};
