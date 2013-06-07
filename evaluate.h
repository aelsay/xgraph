//      evaluate.h
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

#ifndef EVALUATE_H

#define EVALUATE_H

#include <vector>
#include "token.h"

class Evaluate
{
	/**Public functions**/
	public:
	Evaluate() {}
	~Evaluate() {}
	/**This function when called will take a postfix expression, and a variable.
	 * Then it will evaluate the expression by "plugging in" that variable and 
	 * return the answer to the caller. **/
	double evaluate_expression(std::vector<Token> &postfix, double variable);
};

#endif
