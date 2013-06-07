//      postfix.h
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


#ifndef POSTFIX_H

#define POSTFIX_H

#include <vector>
#include "token.h"

class Postfix
{
	public:
	Postfix() {}
	~Postfix() {}
	/**This function takes a parsed infix expression (array) and converts it to
	 * postfix. The expression this function returns will also be an array 
	 * (vector). The returned expression will be used by the evaluate function 
	 * in the Evaluate class. **/
	std::vector<Token> convert_to_postfix(std::vector<Token> &parsed);
};

#endif
