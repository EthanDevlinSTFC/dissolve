// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#pragma once

#include "keywords/data.h"

// Keyword with std::string Data
class StringKeyword : public KeywordData<std::string>
{
    public:
    StringKeyword(std::string_view value = "");
    ~StringKeyword() override;

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
     * Conversion
     */
    public:
    // Return value (as bool)
    bool asBool() override;
    // Return value (as int)
    int asInt() override;
    // Return value (as double)
    double asDouble() override;
    // Return value (as string)
    std::string asString() override;
};
