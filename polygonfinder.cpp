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
#include "polygonfinder.hpp"
#include <algorithm>
#include <iostream>
#include "point.hpp"
#include "constants.hpp"

namespace KDots
{
  
  PolygonFinder::PolygonFinder (Graph& graph, Owner owner)
    : m_graph (graph)
    , m_current (owner)
    , m_stepMap (graph.width (), std::vector<bool> (graph.height (), false))
  {
  }
  
  PolyList
  PolygonFinder::polygons (const Point& point)
  {
    PolyList polygonList;
    findPolygons (point, polygonList);
    int max = 0;
    for (Polygon_ptr ptr : polygonList)
      {
        if (ptr->size () > max)
          {
            max = ptr->size ();
          }
      }
      
    qDebug () << Q_FUNC_INFO << max;
    
    std::remove_if (polygonList.begin (), polygonList.end (), [&max] (Polygon_ptr ptr) { return ptr->size () < max; });
    return polygonList;
  }

  void
  PolygonFinder::findPolygons (const Point& point, PolyList& polygons)
  { 
    std::cout << "CACHE: ";
    for (const Point& i : m_cache)
      {
        std::cout << "[" << i.x () << "," << i.y () << "] ";
      }
      
    std::cout << std::endl;
    

    
    const GraphPoint& graphPoint = m_graph[point];
    
    if (graphPoint.isCaptuted () || graphPoint.owner () != m_current)
      {
        return;
      }
      
    if (m_cache.size () > 3 && point == m_cache.front ())
      {
        qDebug () << Q_FUNC_INFO << "Polygon size: " << m_cache.size ();
        polygons.push_back (Polygon_ptr (new Polygon (m_cache)));
        return;
      }
      
    if (m_stepMap[point.x ()][point.y ()])
      {
        return;
      }
      
    m_cache.push_back (point);
    m_stepMap[point.x ()][point.y ()] = true;
          
    for (int i = 0; i < 8; ++i)
      {
        const int tempx = point.x () + GRAPH_DX[i];
        const int tempy = point.y () + GRAPH_DY[i];
        
        if (tempx < 0 || tempy < 0
            || tempx >= m_graph.width ()
            || tempy >= m_graph.height ())
          {
            continue;
          }
        
        
        findPolygons (Point (tempx, tempy), polygons);
      }
    
    m_cache.pop_back ();
  }
  
}