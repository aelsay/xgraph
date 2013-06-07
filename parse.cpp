//      parse.cpp
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

#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <new>

#include "parse.h"
#include "token.h"

using namespace std;

Parse :: Parse()
{
	_is_function = false;
}

bool Parse :: parse_validate(vector<Token> &parsed)
{
	int bracket_count = 0; /**Makes sure left brackets are equal to right ones**/
	if(parsed.size() == 0)
	{
		return false;
	}
	for(unsigned int pos = 0; pos < parsed.size(); pos++)
	{
		switch(parsed.at(pos).get_category())
		{
			case UNARY: /**If the operator is a UNARY function**/
				if(pos + 1 < parsed.size()) /**Make sure something goes after it**/
				{
					if(parsed.at(pos+1).get_category() != LEFT_BRACKET)
					{
						return false; /**If no left bracket comes after, then false**/
					}
				}
				else /**If nothing comes after, then of course false**/
				{
					return false; /**LOGIC: there must be something after UNARY func**/
				}
				break;
			case BINARY:
				if(pos > 0) /**Checks if there's something valid before it**/
				{
					if(parsed.at(pos-1).get_category() == BINARY ||
						parsed.at(pos-1).get_category() == LEFT_BRACKET)
					{
						return false; /**Another op or a left bracket aren't valid**/
					}
				}
				else
				{
					return false; /**If nothing comes before it: not valid**/
				}
				if(pos + 1 < parsed.size()) /**Check what comes after it**/
				{
					if(parsed.at(pos+1).get_category() == BINARY ||
						parsed.at(pos+1).get_category() == RIGHT_BRACKET)
					{
						return false; /**Another op or a right bracket aren't valid**/
					}
				}
				else /**LOGIC: There must be 2 valid operands for each operator**/
				{
					return false; /**If nothing comes after it: not valid**/
				}
				break;
			case VARIABLE:
			case LITERAL: /**Make sure no 2 literals/variables are after each other**/
				if(pos + 1 < parsed.size())
				{
					if(parsed.at(pos+1).get_category() == VARIABLE ||
						parsed.at(pos+1).get_category() == LITERAL)
					{
						return false;
					}
				}
				break;
			case LEFT_BRACKET: /**Increment bracket count**/
				if(pos + 1 < parsed.size()) // make sure there something inside ()s
				{
					if(parsed.at(pos+1).get_category() == RIGHT_BRACKET)
						return false;
				}
				bracket_count++;
				break;
			case RIGHT_BRACKET: 
				if(pos + 1 < parsed.size())
				{ // Only an operator or any kind of bracket can come after a rb.
					if(parsed.at(pos+1).get_category() == VARIABLE || 
						parsed.at(pos+1).get_category() == LITERAL)
					{
						return false;
					}
				}
				if(bracket_count > 0) // Check if count is greater than zero
					bracket_count--; // Decrement bracket count
				else
					return false; // Otherwise, something is wrong!
				break;
		}
	}
	if(bracket_count != 0)
		return false;
	return true;
}

// This function is called to parse real numbers
void parse_real_number(vector<Token> &parsed, string &expression, unsigned int &pos)
{
	string number; //string that will store the real number
	do {
		number += expression[pos];
		pos++;
	} while (((expression[pos] >= '0' && expression[pos] <= '9') ||
				  expression[pos] == '.') && pos < expression.length());
	pos--; // Adjust pos
	parsed.push_back(Token(number, LITERAL, strtod(number.c_str(), NULL), 0));
}

// This function is called when minus is interpreted as -1 times what's after it
void parse_negative_sign(vector<Token> &parsed)
{
	parsed.push_back(Token("-1", LITERAL, -1, 0));
	/**Times here have a higher precedence than usual (2) because to fix cases
	 * like: x^-x where the program used to interpret it as (x^-1)*x which is 
	 * not the case. By giving this 'special' times a higher precedence than '^' 
	 * the program will calculate -x first then get x^-x. Interpretation will be 
	 * x^(-x). **/
	parsed.push_back(Token("*", BINARY, 0, 2));
}

/**This function is used mainly to save users hassel from typing the times 
 * operator every time. This allows the program to understand expressions like 
 * 2xcos(5x) instead of having the user type 2*x*cos(5*x). **/
void parse_hidden_times(string &expression, unsigned int pos, vector<Token> &parsed)
{
	if(pos + 1 < expression.length())
	{
		if((expression[pos+1] >= '0' && expression[pos+1] <= '9') ||
			 expression[pos+1] == '.' || expression[pos+1] == ')' ||
			 expression[pos+1] == '-' || expression[pos+1] == '+' ||
			 expression[pos+1] == '*' || expression[pos+1] == '/' ||
			 expression[pos+1] == '^');
		else
			parsed.push_back(Token("*", BINARY, 0, 2));
	}
	return;
}

void parse_interpret_minus(vector<Token> &parsed, string &expression, unsigned int &pos)
{
	if(pos + 1 < expression.length()) // Check what's after it
	{
		if((expression[pos+1] >= '0' && expression[pos+1] <= '9') ||
			 expression[pos+1] == '.') // If it's a number
		{
			// Parse real negative number
			parse_real_number(parsed, expression, pos);
			// Check what's after that number
			parse_hidden_times(expression, pos, parsed);
		}
		else // It's (-1) * (what's after it)
			parse_negative_sign(parsed);
	}
}

vector<Token> Parse :: parse_expression(string expression, bool &validate)
{
	validate = true;
	vector<Token> parsed;
	for(unsigned int pos = 0; pos < expression.length(); pos++)
	{
		/**If a space is being read, ignore. Otherwise, parse.**/
		if(expression[pos] == ' ');
		else if(expression[pos] == '(')
			parsed.push_back(Token(expression[pos], LEFT_BRACKET, 0, 0));
		else if(expression[pos] == '+')
			parsed.push_back(Token(expression[pos], BINARY, 0, 1));
		else if(expression[pos] == '-') // Minus case
		{
			if(parsed.size() == 0) // If minus is the first thing being read
				parse_interpret_minus(parsed, expression, pos);
			else if(parsed.size() > 0) //If not, 
			{
				unsigned int prev = parsed.size() - 1; // Prev pos in array
				if(parsed.at(prev).get_category() == LITERAL  ||
					parsed.at(prev).get_category() == VARIABLE ||
					parsed.at(prev).get_category() == RIGHT_BRACKET)
				{
					// The minus sign is just an operator
					parsed.push_back(Token(expression[pos], BINARY, 0, 1));
				}
				else if(parsed.at(prev).get_category() == LEFT_BRACKET)
				{
					// Minus sign is a negative one (-1) by (times) what's after it
					parse_negative_sign(parsed);
				}
				else if(parsed.at(prev).get_category() == BINARY)
				{
					// If what's before the minus sign is an operator
					parse_interpret_minus(parsed, expression, pos);
				}
			}
		}
		else if(expression[pos] == '*' || expression[pos] == '/')
			parsed.push_back(Token(expression[pos], BINARY, 0, 2));
		else if(expression.substr(pos, 3) == "cos" ||
				  expression.substr(pos, 3) == "sin" ||
				  expression.substr(pos, 3) == "tan" ||
				  expression.substr(pos, 3) == "log") // Trig functions 
		{
			parsed.push_back(Token(expression.substr(pos, 3), UNARY, 0, 4));
			pos += 2; // Adjust pos
		}
		else if(expression.substr(pos, 4) == "sqrt" ||
				  expression.substr(pos, 4) == "acos" ||
				  expression.substr(pos, 4) == "asin" ||
				  expression.substr(pos, 4) == "atan") // More Trig functions & sqrt
		{
			parsed.push_back(Token(expression.substr(pos, 4), UNARY, 0, 4));
			pos += 3; // Adjust pos
		}
		else if(expression.substr(pos, 2) == "pi" || 
				  expression.substr(pos, 2) == "PI" ||
				  expression.substr(pos, 2) == "Pi")
		{
			parsed.push_back(Token("π", LITERAL, acos(-1), 0));
			pos += 1;
		}
		else if(expression[pos] == '^')
			parsed.push_back(Token(expression[pos], BINARY, 0, 3));
		else if(expression[pos] == ')')
		{
			parsed.push_back(Token(expression[pos], RIGHT_BRACKET, 0, 5));
			/**In case we have a situation like: (x-2)(x+2), its interpretation
			 * should be (x-2)*(x+2) **/
			parse_hidden_times(expression, pos, parsed);
		}
		else if((expression[pos] >= '0' && expression[pos] <= '9') ||
					expression[pos] == '.') // Real numbers
		{
			parse_real_number(parsed, expression, pos);
			parse_hidden_times(expression, pos, parsed);
		}
		else if(expression[pos] == 'x')
		{
			parsed.push_back(Token(expression[pos], VARIABLE, 0, 0));
			parse_hidden_times(expression, pos, parsed);
			_is_function = true;
		}
		else
		{
			/**Basic cout error if the character being read is invalid**/
			cout << "Syntax Error. Invalid: " << expression[pos] << endl;
			validate = false; // Set validate to false
		}
	}
	validate = parse_validate(parsed);
	return parsed; // Return parsed array to the caller; validate is also passed.
}

bool Parse :: is_function()
{
	return _is_function;
}
