/*
	*** Enum Options Base
	*** src/base/enumoptionsbase.cpp
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

#include "base/charstring.h"
#include "base/enumoptionsbase.h"
#include "base/sysfunc.h"
#include <stddef.h>

// Constructors
EnumOptionsBase::EnumOptionsBase()
{
	name_ = "DummyOptions";

	currentOptionIndex_ = -1;
}

EnumOptionsBase::EnumOptionsBase(const char* name, const EnumOptionsList& options)
{
	name_ = name;
	options_ = options.options();

	currentOptionIndex_ = -1;
}

EnumOptionsBase::EnumOptionsBase(const char* name, const EnumOptionsList& options, int defaultEnumeration)
{
	name_ = name;
	options_ = options.options();;

	currentOptionIndex_ = -1;
	for (int n=0; n<options_.nItems(); ++n) if (options_[n].enumeration() == defaultEnumeration)
	{
		currentOptionIndex_ = n;
		break;
	}
}

// Return name of options (e.g. from source enumeration)
const char* EnumOptionsBase::name() const
{
	return name_;
}

// Return number of options available
int EnumOptionsBase::nOptions() const
{
	return options_.nItems();
}

// Return nth keyword in the list
const char* EnumOptionsBase::keywordByIndex(int index) const
{
	if ((index < 0) || (index >= options_.nItems()))
	{
		Messenger::error("Keyword index %i out of range for EnumOptions '%s'.\n", index, name_);
		return unrecognisedOption_.keyword();
	}

	return options_.constAt(index).keyword();
}

// Return description for the nth keyword in the list
const char* EnumOptionsBase::descriptionByIndex(int index) const
{
	if ((index < 0) || (index >= options_.nItems()))
	{
		Messenger::error("Keyword index %i out of range for EnumOptions '%s'.\n", index, name_);
		return unrecognisedOption_.keyword();
	}

	return options_.constAt(index).description();
}

// Return option by keyword
const EnumOption& EnumOptionsBase::option(const char* keyword) const
{
	for (int n=0; n<options_.nItems(); ++n) if (DissolveSys::sameString(keyword, options_.constAt(n).keyword())) return options_.constAt(n);
	return unrecognisedOption_;
}

// Return current option keyword
const char* EnumOptionsBase::currentOptionKeyword() const
{
	if (currentOptionIndex_ == -1) return "UNDEFINED";

	return options_.constAt(currentOptionIndex_).keyword();
}

// Return current option
const EnumOption& EnumOptionsBase::currentOption() const
{
	if (currentOptionIndex_ == -1) return unrecognisedOption_;

	return options_.constAt(currentOptionIndex_);
}

// Return current option index
int EnumOptionsBase::currentOptionIndex() const
{
	return currentOptionIndex_;
}

// Set current option index
bool EnumOptionsBase::setCurrentOptionIndex(int index)
{
	if ((index < 0) || (index >= options_.nItems())) return Messenger::error("EnumOptions index %i is out of range for '%s'.\n", index, name());

	currentOptionIndex_ = index;

	return true;
}

// Set current option from keyword
bool EnumOptionsBase::setCurrentOption(const char* keyword)
{
	for (int n=0; n<options_.nItems(); ++n) if (DissolveSys::sameString(keyword, options_.constAt(n).keyword()))
	{
		currentOptionIndex_ = n;
		return true;
	}

	return false;
}

// Return whether specified option keyword is valid
bool EnumOptionsBase::isValid(const char* keyword) const
{
	for (int n=0; n<options_.nItems(); ++n) if (DissolveSys::sameString(keyword, options_.constAt(n).keyword())) return true;
	return false;
}

// Raise error, printing valid options
bool EnumOptionsBase::errorAndPrintValid(const char* badKeyword) const
{
	CharString validValueString;
	for (int n=0; n<options_.nItems(); ++n) validValueString += CharString(n == 0 ? "%s" : ", %s", options_.constAt(n).keyword());
	Messenger::error("'%s' is not a valid %s.\nValid options are:  %s", badKeyword, name_, validValueString.get());

	return false;
}
