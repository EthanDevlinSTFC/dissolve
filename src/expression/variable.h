/*
	*** Expression Variable
	*** src/expression/variable.h
	Copyright T. Youngs 2015-2020

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

#ifndef DISSOLVE_EXPRESSION_VARIABLE_H
#define DISSOLVE_EXPRESSION_VARIABLE_H

#include "expression/node.h"
#include "base/charstring.h"

// Variable
class ExpressionVariable : public ExpressionNode
{
	public:
	// Constructor / Destructor
	ExpressionVariable(ExpressionValue value = ExpressionValue(), bool readOnly = false);
	~ExpressionVariable();


	/*
	 * Variable Data
	 */
	protected:
	// Name of the variable
	CharString name_;
	// Value of variable
	ExpressionValue value_;
	// Initial value of new variable
	ExpressionNode* initialValue_;

	public:
	// Set name of variable
	void setName(const char* s);
	// Get name of variable
	const char* name() const;
	// Set initial value expression
	bool setInitialValue(ExpressionNode* node);
	// Return Node corresponding to initial value
	ExpressionNode* initialValue() const;


	/*
	 * Set / Get
	 */
	public:
	// Return value of node
	bool execute(ExpressionValue& value);
	// Set value of node
	bool set(ExpressionValue value);
	// Return value
	const ExpressionValue& value() const;
	// Return pointer to value
	ExpressionValue* valuePointer();


	/*
	 * Inherited Virtuals
	 */
	public:
	// Print node contents
	void nodePrint(int offset, const char* prefix = "");
	// Initialise node
	bool initialise();
};

#endif
