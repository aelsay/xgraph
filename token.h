//      token.h
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

#ifndef TOKEN_H

#define TOKEN_H

#include <string>

enum Category {LITERAL, VARIABLE, BINARY, UNARY, LEFT_BRACKET, RIGHT_BRACKET};

class Token
{
	/**Public**/
	public:
	/**Constructors**/
	Token(std::string input, Category category, double actual_value, int precedence);
	Token(char input, Category category, double actual_value, int precedence);
	/**Get functions can be called to get private variables**/
	std::string get_input();
	Category get_category();
	double get_actual_value();
	int get_precedence();
	/**Private**/
	private:
	/**Set functions are called by the constructor to set private variables**/
	void set_input(char input);
	void set_input(std::string input);
	void set_category(Category category);
	void set_actual_value(double actual_value);
	void set_precedence(int precedence);
	/**Private variables**/
	std::string _input;
	Category _category;
	double _actual_value;
	int _precedence;
};

#endif
