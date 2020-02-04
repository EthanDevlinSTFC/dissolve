/*
	*** Keyword - Module RefList
	*** src/keywords/modulereflist.h
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

#ifndef DISSOLVE_KEYWORD_MODULEREFLIST_H
#define DISSOLVE_KEYWORD_MODULEREFLIST_H

#include "base/charstringlist.h"
#include "keywords/data.h"
#include "templates/reflist.h"

// Forward Declarations
class Module;

// Keyword with Module RefList data
class ModuleRefListKeyword : public KeywordData< RefList<Module>& >
{
	public:
	// Constructors
	ModuleRefListKeyword(RefList<Module>& references, int maxModules = -1);
	ModuleRefListKeyword(RefList<Module>& references, CharStringList allowedModuleTypes, int maxModules = -1);
	// Destructor
	~ModuleRefListKeyword();


	/*
	 * Data
	 */
	private:
	// Module type(s) to allow
	CharStringList moduleTypes_;
	// Maximum number of modules to allow in list (-1 for any number)
	int maxModules_;

	protected:
	// Determine whether current data is 'empty', and should be considered as 'not set'
	bool isDataEmpty() const;

	public:
	// Return the Module type(s) to allow
	const CharStringList& moduleTypes() const;
	// Return maximum number of Modules to allow in the list
	int maxModules() const;


	/*
	 * Arguments
	 */
	public:
	// Return minimum number of arguments accepted
	int minArguments() const;
	// Return maximum number of arguments accepted
	int maxArguments() const;
	// Parse arguments from supplied LineParser, starting at given argument offset
	bool read(LineParser& parser, int startArg, const CoreData& coreData);
	// Write keyword data to specified LineParser
	bool write(LineParser& parser, const char* keywordName, const char* prefix);


	/*
	 * Object Management
	 */
	protected:
	// Prune any references to the supplied Module in the contained data
	void removeReferencesTo(Module* module);
};

#endif
