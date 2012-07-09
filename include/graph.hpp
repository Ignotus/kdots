/*
 * kdots
 * Copyright (C) 2011-2012 Minh Ngo <nlminhtl@gmail.com>
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

#ifndef KDOTS_GRAPH_HPP
#define KDOTS_GRAPH_HPP
#include <vector>
#include "graphpoint.hpp"


namespace KDots
{
  
  class Graph
  {
    std::vector<std::vector<GraphPoint>> m_graph;
  public:
    Graph (int width, int height);

    inline int
    width () const
    {
      return m_graph.size ();
    }
    
    inline int
    height () const
    {
      return m_graph[0].size ();
    }

    inline GraphPoint&
    operator[] (const Point& index)
    {
      return m_graph[index.x ()][index.y ()];
    }
    
    inline const GraphPoint&
    operator[] (const Point& index) const
    {
      return m_graph[index.x ()][index.y ()];
    }
    
    inline std::vector<GraphPoint>&
    operator[] (int index)
    {
      return m_graph[index];
    }
    
    inline const std::vector<GraphPoint>&
    operator[] (int index) const
    {
      return m_graph[index];
    }

    bool addEdge (const Point& first, const Point& second);
    bool removeEdge (const Point& first, const Point& second);
  };

}

#endif
