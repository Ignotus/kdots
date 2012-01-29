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

#include "graph.h"

using namespace std;

Graph::Graph (int width, int height)
: graph (width, vector<GraphPoint> (height))
, widthv (width)
, heightv (height)
{
}

int Graph::height () const
{
	return heightv;
}

int Graph::width () const
{
	return widthv;
}

GraphPoint& Graph::operator[] (const Point<int>& index)
{
	return graph[index.x ()][index.y ()];
}

vector<GraphPoint>& Graph::operator[] (int index)
{
	return graph[index];
}

const std::vector<GraphPoint>& Graph::operator[] (int index) const
{
	return graph[index];
}

bool Graph::addEdge (const Point<int>& first, const Point<int>& second)
{
	graph[first.x ()][first.y ()].edges.addEdge (second);
	graph[second.x ()][second.y ()].edges.addEdge (first);
}

bool Graph::removeEdge (const Point<int>& first, const Point<int>& second)
{
	graph[first.x ()][first.y ()].edges.removeEdge (second);
	graph[second.x ()][second.y ()].edges.removeEdge (first);
}

Graph::iterator Graph::begin ()
{
	return graph.begin ();
}

Graph::const_iterator Graph::begin () const
{
	return graph.begin ();
}

Graph::iterator Graph::end ()
{
	return graph.end ();
}

Graph::const_iterator Graph::end () const
{
	return graph.end ();
}