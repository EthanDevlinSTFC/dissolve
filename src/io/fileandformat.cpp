/*
	*** File/Format Base Class
	*** src/io/fileandformat.cpp
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

#include "io/fileandformat.h"
#include "base/lineparser.h"
#include "base/sysfunc.h"

// Constructors
FileAndFormat::FileAndFormat(int format)
{
	format_ = format;
}

FileAndFormat::FileAndFormat(const char* filename, int format)
{
	filename_ = filename;
	format_ = format;
}

// Destructor
FileAndFormat::~FileAndFormat()
{
}

// Conversion operators
FileAndFormat::operator const char*() const
{
	return filename_.get();
}

/*
 * Formats
 */

// Convert text string to format index
int FileAndFormat::format(const char* s) const
{
	for (int n=0; n<nFormats(); ++n) if (DissolveSys::sameString(s, formats()[n])) return n;

	return nFormats();
}

// Convert format index to text string
const char* FileAndFormat::format(int id) const
{
	if ((id < 0) || (id >= nFormats())) return "???";

	return formats()[id];
}

// Convert format index to nice text string
const char* FileAndFormat::niceFormat(int id) const
{
	if ((id < 0) || (id >= nFormats())) return "???";

	return niceFormats()[id];
}

// Set format index
void FileAndFormat::setFormatIndex(int id)
{
	format_ = id;
}

// Return format index
int FileAndFormat::formatIndex() const
{
	return format_;
}

// Return format string
const char* FileAndFormat::format() const
{
	if ((format_ < 0) || (format_ >= nFormats())) return "???";
	else return format(format_);
}

// Return nice format string
const char* FileAndFormat::niceFormat() const
{
	if ((format_ < 0) || (format_ >= nFormats())) return "???";
	else return niceFormats()[format_];
}

/*
 * Filename
 */

// Set filename / basename
void FileAndFormat::setFilename(const char* filename)
{
	filename_ = filename;
}

// Return filename / basename
const char* FileAndFormat::filename() const
{
	return filename_.get();
}

/*
 * Check
 */

// Return whether a filename has been set
bool FileAndFormat::hasFilename() const
{
	return (!filename_.isEmpty());
}

// Return whether a filename and format have been set
bool FileAndFormat::hasValidFileAndFormat() const
{
	if (filename_.isEmpty()) return false;
	if ((format_ < 0) || (format_ >= nFormats())) return false;

	return true;
}

/*
 * Additional Options
 */

// Return available keywords
KeywordList& FileAndFormat::keywords()
{
	return keywords_;
}

/*
 * Read / Write
 */

// Read format / filename from specified parser
bool FileAndFormat::read(LineParser& parser, int startArg, const char* endKeyword, const CoreData& coreData)
{
	// Convert first argument to format type
	format_ = format(parser.argc(startArg));
	if (format_ == nFormats())
	{
		Messenger::print("Unrecognised format '%s' given for file. Recognised formats are:\n\n", parser.argc(startArg));

		for (int n=0; n<nFormats(); ++n) Messenger::print("  %12s  %s\n", format(n), niceFormat(n));

		return false;
	}

	// Set filename if present
	if (parser.hasArg(startArg+1))
	{
		filename_ = parser.argc(startArg+1);

		// Check that the file exists?
		if (fileMustExist() && (!DissolveSys::fileExists(filename_))) return Messenger::error("Specified file '%s' does not exist.\n", filename_.get());
	}

	// Parse any additional options until we find the end of the block
	while (!parser.eofOrBlank())
	{
		// Read the next line
		if (parser.getArgsDelim() != LineParser::Success) return false;

		// Is this the end of the block?
		if (DissolveSys::sameString(parser.argc(0), endKeyword)) break;

		// Do we recognise the keyword?
		KeywordBase* keyword = keywords_.find(parser.argc(0));
		if (!keyword) return Messenger::error("Unrecognised option '%s' found in file and format block.\n", parser.argc(0));

		// Read in the keyword's data
		if (!keyword->read(parser, 1, coreData)) return Messenger::error("Error reading option '%s'.\n", keyword->name());
	}

	return true;
}

// Write format / filename to specified parser
bool FileAndFormat::writeFilenameAndFormat(LineParser& parser, const char* prefix)
{
	return parser.writeLineF("%s%s  '%s'\n", prefix, format(format_), filename_.get());
}

// Write options and end block
bool FileAndFormat::writeBlock(LineParser& parser, const char* prefix)
{
	return keywords_.write(parser, CharString("%s  ", prefix));
}
