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

#include "graph.hpp"

namespace KDots
{

  Graph::Graph (int width, int height)
    : m_graph (width, std::vector<GraphPoint> (height))
  {
  }

  bool
  Graph::addEdge (const Point& first, const Point& second)
  {
    m_graph[first.x ()][first.y ()].edges ().addEdge (second);
    m_graph[second.x ()][second.y ()].edges ().addEdge (first);
  }

  bool
  Graph::removeEdge (const Point& first, const Point& second)
  {
    m_graph[first.x ()][first.y ()].edges ().removeEdge (second);
    m_graph[second.x ()][second.y ()].edges ().removeEdge (first);
  }

}