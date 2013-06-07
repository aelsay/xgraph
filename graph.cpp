//      graph.cpp
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
//

#include "graph.h"
#include "token.h"
#include "evaluate.h"
#include "parse.h"
#include "postfix.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include <string>
#include <cairo.h>
#include <cmath>
#include <vector>
#include <stack>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <iostream>

using namespace std;

Graph :: Graph(double _x_from, double _y_from, double _x_to, double _y_to)
{
	x_from = _x_from;
	y_from = _y_from;
	x_to = _x_to;
	y_to = _y_to;
	x_range = 20; //I think this should be set by the constructor
	y_range = 20;
	x_points = 2000; //Number of sample points by default
	draw_red = 1.0; //Colors might need to be changed through programs interface
	draw_green = 0.0;
	draw_blue = 0.0;
	axes_red = 0.0;
	axes_green = 0.0;
	axes_blue = 0.0;
	sq_red = 0.45;
	sq_green = 0.45;
	sq_blue = 0.45;
	bg_red = 0.95;
	bg_green = 0.95;
	bg_blue = 0.95;
	line_width = 1.0; //Line width also would be a nice feature option
	graph_scale_update();
}

void Graph :: graph_plot_on_window(GtkWidget *widget, gpointer data)
{
	expression = gtk_entry_get_text(GTK_ENTRY(data));
	g_print("Infix: %s\n", expression); //Print expression to terminal
	cairo_t *cr = gdk_cairo_create(widget->window); //Create cairo context
	graph_plot(cr, true);
	cairo_destroy(cr);
}

string create_file_name(string equation)
{
	string file_name = "";
	for(unsigned int pos = 0; pos < equation.length(); pos++)
	{
		if((equation[pos] >= 'a' && equation[pos] <= 'z') ||
			(equation[pos] >= '0' && equation[pos] <= '9'))
		{
			file_name += equation[pos];
		}
	}
	return file_name;
}

void Graph :: graph_plot_on_surface(GtkWidget *widget, gpointer data)
{
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, x_to, y_to);
	cairo_t *cr = cairo_create(surface);
	graph_plot(cr, false);
	string file_name = create_file_name(expression);
	draw_text(cr, 5, 40, expression, 24, 0.7, 0.2, 0.5);
	if(file_name.length() > 0)
		cairo_surface_write_to_png(surface, file_name.c_str());
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
}

cairo_t * Graph :: graph_plot(cairo_t *cr, bool mode)
{
	cairo_set_source_rgb(cr, bg_red, bg_green, bg_blue);
	if(mode == true)
		cairo_rectangle(cr, x_from, y_from, x_to, y_to);
	else
		cairo_rectangle(cr, 0, 0, x_to, y_to);
	cairo_fill(cr);
	bool validate = false;
	Parse parse; //Create parse instance
	//Declare a vector, and store the parsed expression in it
	vector<Token> parsed_expression = parse.parse_expression(expression, validate);
	is_function = parse.is_function(); //Check if expression is a function
	//Check if the parsed expression is a valid expression
	if(parsed_expression.size() > 0 && validate == true)
	{
		Postfix postfix; //Get Postfix instance
		//Convert the parsed expression to a Postfix expression
		parsed_expression = postfix.convert_to_postfix(parsed_expression);
		cout << "Postfix: "; //Output postfix expression
		for(unsigned int pos = 0; pos < parsed_expression.size() ; pos++)
			cout << parsed_expression.at(pos).get_input() << " ";
		cout << endl;
		draw_graph(cr, parsed_expression);
	}
	else if(parsed_expression.size() == 0)
	{
		draw_text(cr, (x_to + x_from)/6.0, (y_to + y_from)/2.0, "Enter expression in the box above!", 32, 0.8, 0.3, 0.5);
	}
	else
	{
		draw_text(cr, (x_to + x_from)/4.0, (y_to + y_from)/2.0, "Invalid expression!",
			42, 0.7, 0.2, 0.5);
	}
	return cr;
}

void Graph :: graph_x_points_set(double _x_points)
{
	x_points = _x_points;
}

void Graph :: graph_x_range_set(double _x_range)
{
	x_range = _x_range;
}

void Graph :: graph_y_range_set(double _y_range)
{
	y_range = _y_range;
}

void Graph :: graph_line_width_set(double _line_width)
{
	line_width = _line_width;
}

void Graph :: graph_area_set(double _x_from, double _y_from, double _x_to, double _y_to)
{
	x_from = _x_from;
	y_from = _y_from;
	x_to = _x_to;
	y_to = _y_to;
}

void Graph :: graph_set_draw_color(double _draw_red, double _draw_green, double _draw_blue)
{
	draw_red = _draw_red;
	draw_green = _draw_green;
	draw_blue = _draw_blue;
}

void Graph :: graph_set_axes_color(double _axes_red, double _axes_green, double _axes_blue)
{
	axes_red = _axes_red;
	axes_green = _axes_green;
	axes_blue = _axes_blue;
}

void Graph :: graph_set_squares_color(double _sq_red, double _sq_green, double _sq_blue)
{
	sq_red = _sq_red;
	sq_green = _sq_green;
	sq_blue = _sq_blue;
}

void Graph :: graph_set_background_color(double _bg_red, double _bg_green, double _bg_blue)
{
	bg_red = _bg_red;
	bg_green = _bg_green;
	bg_blue = _bg_blue;
}

void Graph :: draw_line(cairo_t *cr, double from_x, double from_y,
	double to_x, double to_y, double red, double green, double blue)
{
	cairo_move_to(cr, from_x, from_y);
	cairo_line_to(cr, to_x, to_y);
	cairo_set_source_rgb(cr, red, green, blue);
	cairo_set_line_width(cr, line_width);
	cairo_stroke(cr);
}

void Graph :: draw_text(cairo_t *cr, double at_x, double at_y, string text,
	double font_size, double red, double green, double blue)
{
	cairo_set_source_rgb(cr, red, green, blue);
	cairo_set_font_size(cr, font_size);
	cairo_move_to(cr, at_x, at_y);
	cairo_show_text(cr, text.c_str());
}

void Graph :: draw_coordinate_system(cairo_t *cr)
{
	/*Draw axes*/
	draw_line(cr, x_origin, y_from, x_origin, y_to);
	draw_line(cr, x_from, y_origin, x_to, y_origin);
	/*Draw graph borders*/
	draw_line(cr, x_from, y_from+1.5, x_to, y_from+1.5, sq_red, sq_green, sq_blue);
	draw_line(cr, x_from, y_to, x_to, y_to, sq_red, sq_green, sq_blue);
	draw_line(cr, x_from+1.5, y_from, x_from+1.5, y_to, sq_red, sq_green, sq_blue);
	draw_line(cr, x_to-1.5, y_from, x_to-1.5, y_to, sq_red, sq_green, sq_blue);
	/*Draw graph squares*/
	double at_x = x_origin + x_scale;
	while(at_x <= x_to)
	{
		draw_line(cr, at_x, y_from, at_x, y_to, sq_red, sq_green, sq_blue);
		at_x += x_scale;
	}
	at_x = x_origin - x_scale;
	while(at_x >= x_from)
	{
		draw_line(cr, at_x, y_from, at_x, y_to, sq_red, sq_green, sq_blue);
		at_x -= x_scale;
	}
	double at_y = y_origin + y_scale;
	while(at_y <= y_to)
	{
		draw_line(cr, x_from, at_y, x_to, at_y, sq_red, sq_green, sq_blue);
		at_y += y_scale;
	}
	at_y = y_origin - y_scale;
	while(at_y >= y_from)
	{
		draw_line(cr, x_from, at_y, x_to, at_y, sq_red, sq_green, sq_blue);
		at_y -= y_scale;
	}
}

void Graph :: draw_graph(cairo_t *cr, vector<Token> &expression)
{
	graph_scale_update(); //Update graph scale
	draw_coordinate_system(cr); //Draw coordinate system
	double x_pos = x_axis_min;
	Evaluate evaluate; //Get eval instance
	time_t start, end; //Declare time types
	time(&start); //Start timer
	while(x_pos <= x_axis_max)
	{
		long double from_x = x_pos*x_scale + x_origin;
		long double from_y = -1.0*evaluate.evaluate_expression(expression, x_pos) * y_scale + y_origin;
		long double to_x = (x_pos + x_distance)*x_scale + x_origin;
		long double to_y = -1.0*evaluate.evaluate_expression(expression, x_pos+x_distance) * y_scale + y_origin;
		/*If graph is within area bounds, then draw; otherwise, skip.*/
		if(from_y >= y_from && to_y >= y_from &&
			from_y <= y_to && to_y <= y_to && 
			from_x >= x_from && from_x <= x_to && 
			to_x >= x_from && to_x <= x_to &&  
			isnan(from_y) == false && isnan(to_y) == false &&
			isinf(from_y) == false && isinf(to_y) == false)
		{
			draw_line(cr, from_x, from_y, to_x, to_y, draw_red, draw_green, draw_blue);
		}
		x_pos += x_distance; //move to next point on x-axis.
	}
	time(&end); //Stop timer
	double graphing_time = difftime(end, start); //Calculate time
	cout << "Graphing time: " << graphing_time << "s" << endl;
	if(graph_is_function_get() == false)
		constant = evaluate.evaluate_expression(expression, -1);
}

void Graph :: graph_scale_update()
{
	x_axis_min = -1.0*x_range/2.0;
	x_axis_max = -x_axis_min;
	y_axis_min = -1.0*y_range/2.0;
	y_axis_max = -y_axis_min;
	x_origin = (x_from + x_to)/2.0;
	y_origin = (y_from + y_to)/2.0;
	x_scale = (x_from + x_to)/x_range;
	y_scale = (y_from + y_to)/y_range;
	x_distance = x_range/x_points;
}

double Graph :: graph_x_from_area_get()
{
	return x_from;
}

double Graph :: graph_y_from_area_get()
{
	return y_from;
}

double Graph :: graph_x_to_area_get()
{
	return x_to;
}

double Graph :: graph_y_to_area_get()
{
	return y_to;
}

double Graph :: graph_x_points_get()
{
	return x_points;
}

double Graph :: graph_x_range_get()
{
	return x_range;
}

double Graph :: graph_y_range_get()
{
	return y_range;
}

double Graph :: graph_x_scale_get()
{
	return x_scale;
}

double Graph :: graph_y_scale_get()
{
	return y_scale;
}

double Graph :: graph_x_axis_min_get()
{
	return x_axis_min;
}

double Graph :: graph_y_axis_min_get()
{
	return y_axis_min;
}

double Graph :: graph_x_axis_max_get()
{
	return x_axis_max;
}

double Graph :: graph_y_axis_max_get()
{
	return y_axis_max;
}

double Graph :: graph_x_distance_get()
{
	return x_distance;
}

double Graph :: graph_line_width_get()
{
	return line_width;
}

double Graph :: graph_constant_get()
{
	return constant;
}

bool Graph :: graph_is_function_get()
{
	return is_function;
}
