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
#include "dottable.hpp"
#include <QDebug>
#include "polygonfinder.hpp"

namespace KDots
{

  DotTable::DotTable (int width, int height, GameMode mode, Owner owner)
    : m_graph (width, height)
    , m_steps (mode == DEFAULT_MODE
        ? new StepQueue (owner)
        : new ExtraStepQueue (owner))
  {
  }
  
  namespace
  {
    bool
    isInPolygon (Polygon_ptr polygon, const Point& point)
    {
      int i, k;
      Polygon::const_iterator itr = polygon->begin ();
      Polygon::const_iterator itrEnd = polygon->end ();

      i = k = 0;

      while (itr != itrEnd)
        {
          if (itr->y () != point.y ())
            {
              ++itr;
              continue;
            }
          else
            {
              ++k;
            }

          const Point& prevPoint = itr-- == polygon->begin ()
              ? polygon->back ()
              : *itr;

          ++itr;

          const Point& nextPoint = ++itr == polygon->end ()
              ? polygon->front ()
              : *itr;

          --itr;

          if (itr->x () < point.x () && prevPoint.y () != nextPoint.y ())
            {
              ++i;
            }

          ++itr;
        }

      return k != i && i % 2;
    }
  }

  void
  DotTable::pushPoint (const Point& point)
  {
    GraphPoint& currentPoint = m_graph[point];
    if (currentPoint.owner () != NONE
        || currentPoint.isCaptuted ())
      {
        return;
      }

    const Owner current = m_steps->getCurrentOwner ();

    currentPoint.setOwner(current);

    m_steps->addPoint (point);

    PolygonFinder finder (m_graph, current);

    //O(n)
    const PolyList& polyList = finder.polygons (point);

    const std::list<Point>& points = m_steps->getPoints (StepQueue::other (current));
    
    if (!points.size () || !polyList.size ())
      {
        m_steps->nextStep ();
        return;
      }
      
    std::vector<bool> filled (polyList.size (), false);
    
    for (int k = 0; k < m_graph.width (); ++k)
      {
        int j;
        for (j = 0; j < m_graph.height (); ++j)
          {
            const GraphPoint& gpoint = m_graph[k][j];
            if (gpoint.isCaptuted () || gpoint.owner () == current)
              {
                continue;
              }
              
            for (int i = 0; i < polyList.size (); ++i)
              {
                const Point newPoint (k, j);
                if (isInPolygon (polyList[i], newPoint))
                  {
                    if (gpoint.owner () == StepQueue::other (current))
                      {
                        filled[i] = true;
                        m_steps->addCaptured ();
                      }
                      
                    m_graph[newPoint].capture ();
                    break;
                  }
              }
          }
      }
      
    drawPolygon (polyList, filled);
    
    m_steps->nextStep ();
    
  }

  void
  DotTable::drawPolygon (PolyList polygons, const std::vector<bool>& filled)
  {
    for (int i = 0; i < polygons.size (); ++i)
      {
        if (!filled[i])
          {
            continue;
          }
          
        Polygon_ptr polygon = polygons[i];
          
        Polygon::const_iterator itr = polygon->begin ();
        Point prevPoint = polygon->back ();
        for (; itr != polygon->end (); ++itr)
          {
            m_graph.addEdge (prevPoint, *itr);
            prevPoint = *itr;
          }
        }
      }
}
