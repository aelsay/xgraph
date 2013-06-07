//      evaluate.cpp
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

#include <cmath>
#include <vector>
#include <stack>
#include "token.h"
#include "evaluate.h"

using namespace std;

double Evaluate :: evaluate_expression(vector<Token> &postfix, double variable)
{
	stack<double> output; /**Stores output values**/
	double operand1, operand2; /**Store operand variables**/
	/**Go through every item in postfix vector**/
	for (unsigned int pos = 0 ; pos < postfix.size() ; pos++)
	{
		/**Add literals to the stack**/
		if(postfix.at(pos).get_category() == LITERAL)
			output.push(postfix.at(pos).get_actual_value());
		else if(postfix.at(pos).get_category() == BINARY)
		{
			/**Get 2 numbers from the stack**/
			if(output.size() > 1)
			{
				operand2 = output.top();
				output.pop();
				operand1 = output.top();
				output.pop();
				/**Perform right operation**/
				if(postfix.at(pos).get_input() == "+")
					output.push(operand1 + operand2);
				else if(postfix.at(pos).get_input() == "-")
					output.push(operand1 - operand2);
				else if(postfix.at(pos).get_input() == "*")
					output.push(operand1 * operand2);
				else if(postfix.at(pos).get_input() == "/")
					output.push(operand1 / operand2);
				else if (postfix.at(pos).get_input() == "^")
					output.push(pow(operand1, operand2));
			}
		}
		else if(postfix.at(pos).get_category() == UNARY)
		{
			if(output.size() > 0)
			{
				/**Get top number on the stack**/
				operand1 = output.top();
				output.pop();
				/**Perform right operation**/
				if(postfix.at(pos).get_input() == "cos")
					output.push(cos (operand1));
				else if(postfix.at(pos).get_input() == "sin")
					output.push(sin (operand1));
				else if(postfix.at(pos).get_input() == "tan")
					output.push(tan (operand1));
				else if(postfix.at(pos).get_input() == "log")
					output.push(log (operand1));
				else if(postfix.at(pos).get_input() == "sqrt")
					output.push(sqrt (operand1));
				else if(postfix.at(pos).get_input() == "acos")
					output.push(acos (operand1));
				else if(postfix.at(pos).get_input() == "asin")
					output.push(asin (operand1));
				else if(postfix.at(pos).get_input() == "atan")
					output.push(atan (operand1));
			}
		}
		else if (postfix.at(pos).get_category() == VARIABLE)
		{
			if (postfix.at(pos).get_input() == "x")
				output.push(variable); /**Push variable to the stack**/
		}
	}
	return output.top(); /**Return answer**/
}
