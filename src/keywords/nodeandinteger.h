// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#pragma once

#include "base/lineparser.h"
#include "keywords/data.h"
#include "keywords/nodebase.h"
#include "procedure/nodes/node.h"
#include <tuple>

// Keyword with ProcedureNode and integer index
class NodeAndIntegerKeyword : public NodeKeywordBase, public KeywordData<std::pair<const ProcedureNode *, int>>
{
    public:
    NodeAndIntegerKeyword(ProcedureNode *parentNode, ProcedureNode::NodeType nodeType, bool onlyInScope,
                          const ProcedureNode *node = nullptr, int index = 0);
    NodeAndIntegerKeyword(ProcedureNode *parentNode, ProcedureNode::NodeClass nodeClass, bool onlyInScope,
                          const ProcedureNode *node = nullptr, int index = 0);
    ~NodeAndIntegerKeyword() override = default;

    /*
     * Arguments
     */
    public:
    // Return minimum number of arguments accepted
    int minArguments() const override;
    // Return maximum number of arguments accepted
    int maxArguments() const override;
    // Parse arguments from supplied LineParser, starting at given argument offset
    bool read(LineParser &parser, int startArg, const CoreData &coreData) override;
    // Write keyword data to specified LineParser
    bool write(LineParser &parser, std::string_view keywordName, std::string_view prefix) const override;

    /*
     * Object Management
     */
    protected:
    // Prune any references to the supplied ProcedureNode in the contained data
    void removeReferencesTo(ProcedureNode *node) override;
};
