//      graph.h
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

#ifndef GRAPH_H

#define GRAPH_H

#include <gtk/gtk.h>
#include <cairo.h>
#include <vector>
#include <cmath>
#include <stack>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include "token.h"

using namespace std;

class Graph
{
	public:
	Graph(double _x_from, double _y_from, double _x_to, double _y_to);
	/**Plot functions: one plots on a window, the other on a cairo surface.**/
	void graph_plot_on_window(GtkWidget *widget, gpointer data);
	void graph_plot_on_surface(GtkWidget *widget, gpointer data);
	/**Set functions for x & y range, line width, graph area, and colors.**/
	void graph_x_points_set(double _x_points);
	void graph_x_range_set(double _x_range);
	void graph_y_range_set(double _y_range);
	void graph_line_width_set(double _line_width);
	void graph_area_set(double _x_from, double _y_from, double _x_to, double _y_to);
	void graph_set_draw_color(double _draw_red, double _draw_green, double _draw_blue);
	void graph_set_axes_color(double _axes_red, double _axes_green, double _axes_blue);
	void graph_set_squares_color(double _sq_red, double _sq_green, double _sq_blue);
	void graph_set_background_color(double _bg_red, double _bg_green, double _bg_blue);
	/**Clear background function**/
	void graph_clear_background();
	/**Get functions to access graph area limits, x & y range, x & y scale, 
	 * x distance and line width.**/
	double graph_x_from_area_get();
	double graph_y_from_area_get();
	double graph_x_to_area_get();
	double graph_y_to_area_get();
	double graph_x_points_get();
	double graph_x_range_get();
	double graph_y_range_get();
	double graph_x_scale_get();
	double graph_y_scale_get();
	double graph_x_axis_min_get();
	double graph_y_axis_min_get();
	double graph_x_axis_max_get();
	double graph_y_axis_max_get();
	double graph_x_distance_get();
	double graph_line_width_get();
	double graph_constant_get();
	bool graph_is_function_get();
	/**Private variables**/
	private:
	/**graph_plot function plots the graph and a cairo_t context and returns
	 * that cairo context to any of the plot functions that call it.**/
	cairo_t * graph_plot(cairo_t *cr, bool mode);
	/**draw_line will draw lines when called**/
	void draw_line(cairo_t *cr, double from_x, double from_y, double to_x, 
		double to_y, double red = 0, double green = 0, double blue = 0);
	/**draws text**/
	void draw_text(cairo_t *cr, double at_x, double at_y, string text, 
		double font_size,double red = 0, double green = 0, double blue = 0);
	/**draws the coordinate system**/
	void draw_coordinate_system(cairo_t *cr);
	/**Prepares to draw the graph; gets the equation**/
	void draw_graph(cairo_t *cr, vector<Token> &expression);
	/**Scale graph squares when called**/
	void graph_scale_update();
	/**Graph variables**/
	double x_from, y_from, x_to, y_to;
	double area_width, area_height;
	double x_range, y_range;
	double x_axis_min, x_axis_max, y_axis_min, y_axis_max;
	double x_scale, y_scale, x_distance, x_points;
	double x_origin, y_origin;
	double draw_red, draw_green, draw_blue;
	double axes_red, axes_green, axes_blue;
	double sq_red, sq_green, sq_blue;
	double bg_red, bg_green, bg_blue;
	double line_width;
	const gchar* expression;
	bool is_function;
	double constant;
};

#endif
