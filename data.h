//      data.h
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

#ifndef GRAPH_INSTANCE_H

#define GRAPH_INSTANCE_H

#include "graph.h"

class Data
{
	public:
	static Data *get_data_instance(double _x_from, double _y_from, double _x_to, double _y_to);
	Graph *get_graph_instance();
	~Data();
	protected:
	Data(double _x_from, double _y_from, double _x_to, double _y_to);
	private:
	static Data *data;
	Graph *graph;	
};

#endif
