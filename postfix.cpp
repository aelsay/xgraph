//      postfix.cpp
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

#include <vector>
#include <stack>
#include <cmath>
#include "token.h"
#include "postfix.h"

using namespace std;

vector<Token> Postfix :: convert_to_postfix(vector<Token> &parsed)
{
	stack<Token> Operator; /**Stack to hold operators**/
	vector<Token> postfix; /**Vector to store the postfix expression**/
	for(unsigned int pos = 0; pos < parsed.size(); pos++)
	{
		if(parsed.at(pos).get_category() == LEFT_BRACKET)
		{
			Operator.push(parsed.at(pos)); /**Push left brackets to the stack**/
		}
		else if(parsed.at(pos).get_category() == LITERAL ||
				  parsed.at(pos).get_category() == VARIABLE)
		{
			postfix.push_back(parsed.at(pos)); /**Numbers are pushed to vector**/
		}
		else if(parsed.at(pos).get_category() == BINARY ||
				  parsed.at(pos).get_category() == UNARY)
		{
			/**While the stack isn't empty, and token's precedence <= top**/
			while(Operator.empty() == false &&
					Operator.top().get_precedence() >= parsed.at(pos).get_precedence())
			{
				postfix.push_back(Operator.top()); /**Push top to postfix vector**/
				Operator.pop(); /**Pop it off the stack**/
			}
			Operator.push(parsed.at(pos));
		}
		else if(parsed.at(pos).get_category() == RIGHT_BRACKET)
		{
			while (Operator.empty() == false && /**Similar to above**/
					 Operator.top().get_category() != LEFT_BRACKET)
			{
				postfix.push_back(Operator.top());
				Operator.pop();
			}
			Operator.pop();
		}
	}
	while (Operator.empty() == false) /**Pop the rest of the operators off the stack**/
	{
		postfix.push_back(Operator.top());
		Operator.pop();
	}
	return postfix; /**Return postfix expression**/
}
