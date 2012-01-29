/*
 * kdots
 * Copyright (C) 2011 Minh Ngo <nlminhtl@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KDOTS_GRAPH_H
#define KDOTS_GRAPH_H

#include <vector>
#include "graphpoint.h"

class Graph
{
	std::vector< std::vector<GraphPoint> > graph;
	int widthv, heightv;
public:
	typedef std::vector< std::vector<GraphPoint> >::const_iterator const_iterator;
	typedef std::vector< std::vector<GraphPoint> >::iterator iterator;
	
	Graph (int width, int height);
	
	int width () const;
	int height () const;
	
	iterator begin ();
	const_iterator begin () const;
	iterator end ();
	const_iterator end () const;
	
	GraphPoint& operator[] (const Point<int>& index);
	std::vector<GraphPoint>& operator[] (int index);
	const std::vector<GraphPoint>& operator[] (int index) const;
	
	bool addEdge (const Point<int>& first, const Point<int>& second);
	bool removeEdge (const Point<int>& first, const Point<int>& second);
};

#endif // KDOTS_GRAPH_H
