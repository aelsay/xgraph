//      token.cpp
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

#include "token.h"

Token :: Token(std::string input, Category category, double actual_value, int precedence)
{
	set_input(input);
	set_category(category);
	set_actual_value(actual_value);
	set_precedence(precedence);
}

Token :: Token(char input, Category category, double actual_value, int precedence)
{
	set_input(input);
	set_category(category);
	set_actual_value(actual_value);
	set_precedence(precedence);
}

/**Get functions can be called to get private variables**/
std::string Token :: get_input()
{
	return _input;
}

Category Token :: get_category()
{
	return _category;
}

double Token :: get_actual_value()
{
	return _actual_value;
}

int Token :: get_precedence()
{
	return _precedence;
}

/**Set functions are called by the constructor to set private variables**/
void Token :: set_input(char input)
{
	_input = input;
}

void Token :: set_input(std::string input)
{
	_input = input;
}

void Token :: set_category(Category category)
{
	_category = category;
}

void Token :: set_actual_value(double actual_value)
{
	_actual_value = actual_value;
}

void Token :: set_precedence(int precedence)
{
	_precedence = precedence;
}
