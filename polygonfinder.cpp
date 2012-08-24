/*
 * KDots
 * Copyright (c) 2011-2012 Minh Ngo <nlminhtl@gmail.com>
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
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "polygonfinder.hpp"
#include <algorithm>
#include <iostream>
#include <KDebug>
#include "point.hpp"
#include "constants.hpp"
#include "graph.hpp"

namespace KDots
{
  
	PolygonFinder::PolygonFinder (Graph& graph, Owner owner)
		: m_graph (graph)
		, m_current (owner)
		, m_stepMap (graph.width (), std::vector<bool> (graph.height (), false))
	{
	}

	namespace
	{
		float area (const Polygon& polygon)
		{
			float res = 0;
			Point prevPoint = polygon.back ();
			for (Polygon::const_iterator itr = polygon.begin (), itrEnd = polygon.end ();
					 itr != itrEnd; ++itr)
			{
				res += (itr->x () - prevPoint.x ()) * float (itr->y () + prevPoint.y ()) / 2; 
				prevPoint = *itr;
			} 
			
			return std::abs (res);
		}
		
		float maxSize (const PolyList& polygonList)
		{
			float max = 0;

			for (const Polygon_ptr& ptr : polygonList)
			{
				const float currArea = area (*ptr);
				ptr->setArea (currArea);
				if (currArea > max)
					max = currArea;
			}

			return max;
		}
	}

	const PolyList& PolygonFinder::operator() (const Point& point)
	{
		findPolygons (point);

		const float max = maxSize (m_polygons);
		m_polygons.erase (std::remove_if (m_polygons.begin (), m_polygons.end (),
						[&max] (const Polygon_ptr& ptr)
						{
							return ptr->area () < max;
						}),
				m_polygons.end ());
		
		return m_polygons;
	}

	void PolygonFinder::findPolygons (const Point& point)
	{
		const GraphPoint& graphPoint = m_graph[point];

		if (graphPoint.isCaptured () || graphPoint.owner () != m_current)
			return;

		if (m_cache.size () > 3 && point == m_cache.front ())
		{
			m_polygons.push_back (Polygon_ptr (new Polygon (m_cache)));
			return;
		}

		if (m_stepMap[point.x ()][point.y ()])
			return;

		m_cache.push_back (point);
		m_stepMap[point.x ()][point.y ()] = true;

		for (int i = 0; i < 8; ++i)
		{
			const int tempx = point.x () + GRAPH_DX[i];
			const int tempy = point.y () + GRAPH_DY[i];

			if (tempx < 0 || tempy < 0
					|| static_cast<std::size_t> (tempx) >= m_graph.width ()
					|| static_cast<std::size_t> (tempy) >= m_graph.height ())
				continue;

			findPolygons (Point (tempx, tempy));
		}

		m_cache.pop_back ();
	}
}