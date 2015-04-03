/*
 * KDots
 * Copyright (c) 2011, 2012, 2014, 2015 Minh Ngo <minh@fedoraproject.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "polygonfinder.hpp"

#include <algorithm>

#include "point.hpp"
#include "constants.hpp"
#include "graph.hpp"

namespace KDots
{
  PolygonFinder::PolygonFinder(const Graph& graph, Owner owner,
                               const std::vector<Point>& additionalPoints)
    : m_graph(graph)
    , m_current(owner)
    , m_stepMap(graph.width(), std::vector<bool>(graph.height(), false))
    , m_additionalPoints(additionalPoints)
  {
  }

  namespace
  {
    bool areaComp(const PolyList::value_type& p1, const PolyList::value_type& p2)
    {
      return p1->area() < p2->area();
    }

    int maxSize(const PolyList& polygonList)
    {
      return (*std::max_element(polygonList.begin(), polygonList.end(), areaComp))->area();
    }
  }

  const PolyList& PolygonFinder::operator()(const Point& point)
  {
    m_first = point;
    findPolygons(point);

    if (m_polygons.empty())
      return m_polygons;

    const int max = maxSize(m_polygons);

    auto removePred = [&max](const Polygon_ptr& ptr) {
      return ptr->area() < max;
    };

    m_polygons.erase(std::remove_if(m_polygons.begin(), m_polygons.end(), removePred),
                     m_polygons.end());

    return m_polygons;
  }
  
  bool PolygonFinder::isAdditionalPoint(const Point& point) const
  {
    for (const Point& pi : m_additionalPoints)
      if (pi == point)
        return true;
    
    return false;
  }

  namespace
  {
    const Point GRAPH_OFFSET[DIRECTION_COUNT] = {
      { 0,  1},
      { 1,  1},
      { 1,  0},
      { 1, -1},
      { 0, -1},
      {-1, -1},
      {-1,  0},
      {-1,  1}
    };
  }

  void PolygonFinder::findPolygons(const Point& point)
  {
    if (m_cache.size() > 3 && point == m_cache.front())
    {
      m_polygons.push_back(std::make_shared<Polygon>(m_cache));
      return;
    }

    if (m_stepMap[point.m_x][point.m_y])
      return;

    m_cache.push_back(point);
    m_stepMap[point.m_x][point.m_y] = true;

    for (int i = 0; i < DIRECTION_COUNT; ++i)
    {
      const Point newPoint(point + GRAPH_OFFSET[i]);

      if (!m_graph.isValid(newPoint))
        continue;

      const GraphPoint& graphPoint = m_graph[newPoint];

      if (!isAdditionalPoint(newPoint) && newPoint != m_first
          && (graphPoint.isCaptured() || graphPoint.owner() != m_current))
        continue;

      findPolygons(newPoint);
    }

    m_cache.pop_back();
  }
}