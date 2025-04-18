// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#include "keywords/valuestore.h"
#include "base/lineparser.h"
#include "classes/valuestore.h"

ValueStoreKeyword::ValueStoreKeyword(ValueStore &data1DStore)
    : KeywordData<ValueStore &>(KeywordBase::ValueStoreData, data1DStore)
{
}

ValueStoreKeyword::~ValueStoreKeyword() = default;

/*
 * Arguments
 */

// Return minimum number of arguments accepted
int ValueStoreKeyword::minArguments() const
{
    // Must have reference data name and format as a minimum
    return 2;
}

// Return maximum number of arguments accepted
int ValueStoreKeyword::maxArguments() const
{
    // Filename, name of data, and other args
    return 99;
}

// Parse arguments from supplied LineParser, starting at given argument offset
bool ValueStoreKeyword::read(LineParser &parser, int startArg, const CoreData &coreData)
{
    Messenger::print("Reading test data '{}' from file '{}' (format={})...\n", parser.argsv(startArg),
                     parser.argsv(startArg + 2), parser.argsv(startArg + 1));

    if (!data_.addData(parser.argsv(startArg), parser, startArg + 1, fmt::format("End{}", name()), coreData))
        return Messenger::error("Failed to add data.\n");

    set_ = true;

    return true;
}

// Write keyword data to specified LineParser
bool ValueStoreKeyword::write(LineParser &parser, std::string_view keywordName, std::string_view prefix) const
{
    // Loop over list of one-dimensional data
    for (const auto &[tag, data, format] : data_.data())
    {
        if (!format.writeFilenameAndFormat(parser, fmt::format("{}{}  '{}'  ", prefix, keywordName, tag)))
            return false;
        if (!format.writeBlock(parser, fmt::format("{}  ", prefix)))
            return false;
        if (!parser.writeLineF("{}End{}\n", prefix, name()))
            return false;
    }

    return true;
}
