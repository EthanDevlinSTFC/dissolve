// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#include "base/enumoptionsbase.h"
#include "base/sysfunc.h"

/*
 * Search
 */

// Return index of matching keyword, if it exists
std::optional<int> EnumOptionsBase::keywordIndex(std::string_view keyword) const
{
    for (auto n = 0; n < nOptions(); ++n)
        if (DissolveSys::sameString(keywordByIndex(n), keyword))
            return n;

    return std::nullopt;
}
