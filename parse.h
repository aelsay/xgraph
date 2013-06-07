//      parse.h
//
//      Copyright (C) 2010 Ali ElSayed <aelsay@gmail.com>
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, see <http://www.gnu.org/licenses/>.

#ifndef PARSE_H

#define PARSE_H

#include <string>
#include <vector>

#include "token.h"

class Parse
{
	/**Public functions**/
	public:
	Parse();
	~Parse() {}
	/**Parse expression is called to parse the expression. It takes a string 
	 * which is the math equation, breaks it into tokens and returns it as an 
	 * array of tokens (or a vector of tokens). It also calls validate to check 
	 * if the parsed equation is true. If it is, the bool passed to the function 
	 * will be true; otherwise, false. **/
	std::vector<Token> parse_expression(std::string expression, bool &validate);
	/**Returns true if parsed expression has a variable. Otherwise, false.**/
	bool is_function();
	/**This function checks if the parsed equation is valid**/
	private:
	bool parse_validate(std::vector<Token> &parsed);
	bool _is_function;
};

#endif
