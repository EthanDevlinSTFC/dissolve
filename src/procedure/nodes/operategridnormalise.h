// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#pragma once

#include "procedure/nodes/operatebase.h"

// Operate Grid Normalise Node
class OperateGridNormaliseProcedureNode : public OperateProcedureNodeBase
{
    public:
    OperateGridNormaliseProcedureNode();
    ~OperateGridNormaliseProcedureNode() override = default;

    /*
     * Data Target (implements virtuals in OperateProcedureNodeBase)
     */
    public:
    // Operate on Data1D target
    bool operateData1D(ProcessPool &procPool, Configuration *cfg) override;
    // Operate on Data2D target
    bool operateData2D(ProcessPool &procPool, Configuration *cfg) override;
    // Operate on Data3D target
    bool operateData3D(ProcessPool &procPool, Configuration *cfg) override;
};
