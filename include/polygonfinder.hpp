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
#ifndef KDOTS_POLYGONFINDER_HPP
#define KDOTS_POLYGONFINDER_HPP
#include <memory>
#include <list>
#include <QDebug>
#include "graph.hpp"

namespace KDots
{
  class Point;
  
  typedef std::list<Point> Polygon;
  typedef std::shared_ptr<Polygon> Polygon_ptr;
  typedef std::vector<Polygon_ptr> PolyList;
  
  class PolygonFinder
  {
    Graph& m_graph;
    Owner m_current;
    std::list<Point> m_cache;
    std::vector<std::vector<bool>> m_stepMap;
  public:
    PolygonFinder (Graph& graph, Owner owner);
    ~PolygonFinder ()
    {
      qDebug () << Q_FUNC_INFO << __LINE__;
    }
    
    // O(n)
    PolyList polygons (const Point& point);
  private:
    void findPolygons (const Point& point, PolyList& polygons);
  };

}

#endif