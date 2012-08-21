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
#include "dottable.hpp"
#include "polygonfinder.hpp"
#include "stepqueue.hpp"

namespace KDots
{
	DotTable::DotTable (const GameConfig& config, QObject *parent)
		: QObject (parent)
		, m_graph (new Graph (config.m_width, config.m_height))
		, m_steps (config.m_mode == DEFAULT_MODE
				? new StepQueue (config.m_firstOwner)
				: new ExtraStepQueue (config.m_firstOwner))
		, m_config (config)
	{
	}
	
	GameConfig DotTable::gameConfig () const
	{
		return m_config;
	}

	namespace
	{
		Point getPrevPoint (Polygon_ptr& polygon, Polygon::const_iterator current)
		{
			const int currentY = current->y ();
			for (Polygon::const_iterator prev = current;;)
			{
				if (prev == polygon->begin ())
					prev = --polygon->end ();
				else
					--prev;
				
				if (prev->y () != currentY)
					return *prev;
			}
		}
		
		Point getNextPoint (Polygon_ptr& polygon, int& shift, Polygon::const_iterator current)
		{
			const int currentY = current->y ();
			shift = 0;
			for (Polygon::const_iterator next = current;;)
			{
				++shift;
				if (next == --polygon->end ())
					next = polygon->begin ();
				else
					++next;
				
				if (next->y () != currentY)
					return *next;
			}
		}
		
		bool isInPolygon (Polygon_ptr polygon, const Point& point)
		{
			// k - a count of points in the same line with "point" object
			// i - crosses count
			int i = 0, shift;

			Polygon::const_iterator itr = polygon->begin (), itrEnd = polygon->end ();
			while (itr != itrEnd)
			{
				if (itr->y () != point.y ())
				{
					++itr;
					continue;
				}

				const Point& prevPoint = getPrevPoint (polygon, itr);
				const Point& nextPoint = getNextPoint (polygon, shift, itr);

				if (itr->x () < point.x () && prevPoint.y () != nextPoint.y () && shift == 1)
					++i;
				
				++itr;
			}

			return i % 2;
		}
	}

	void DotTable::pushPoint (const Point& point)
	{
		Graph& graph = *m_graph;
		GraphPoint& currentPoint = graph[point];

		if (currentPoint.owner () != NONE || currentPoint.isCaptured ())
			return;

		const Owner current = m_steps->getCurrentOwner ();

		currentPoint.setOwner (current);

		m_steps->addPoint (point);

		PolygonFinder findPolygon (graph, current);

		//O(n)
		const PolyList& polyList = findPolygon (point);

		const std::list<Point>& points = m_steps->getPoints (StepQueue::other (current));
		if (points.empty () || polyList.empty ())
		{
			m_steps->nextStep ();
			emit nextPlayer (point);
			return;
		}
		
		const Owner otherOwner = StepQueue::other (current);
	
		const std::list<Point>& otherOwnerPoints = m_steps->getPoints (otherOwner);
		for (const Point& p : otherOwnerPoints)
		{
			GraphPoint& gpoint = graph[p];
			if (gpoint.isCaptured ())
				continue;
			
			for (Polygon_ptr polygon : polyList)
			{
				if (isInPolygon (polygon, p))
				{
					if (gpoint.owner () == otherOwner)
					{
						polygon->setFilled (true);
						m_steps->addCaptured ();
					}
						
					gpoint.capture ();
					break;
				}
			}
		}
		
		for (Graph::iterator itr = graph.begin (), itrEnd = graph.end ();
				itr != itrEnd; ++itr)
		{
			if (itr->isCaptured () || itr->owner () != NONE)
				continue;
			
			for (Polygon_ptr polygon : polyList)
			{
				const Point& newPoint = itr.point ();

				if (isInPolygon (polygon, newPoint) && polygon->isFilled ())
				{
					itr->capture ();
					break;
				}
			}	
		}
		
		drawPolygon (polyList);
		m_steps->nextStep ();
		emit nextPlayer (point);
	}

	//Hardcore undo process
	void DotTable::undo ()
	{
		m_graph.reset (new Graph (m_config.m_width, m_config.m_height));
		m_polygons.clear ();
		std::list<Point> points (m_steps->getAllPoints ());
		
		if (!points.empty ())
			points.pop_back ();
		m_steps->clear ();

		for (const Point& point : points)
			pushPoint (point);
	}

	void DotTable::drawPolygon (PolyList polygons)
	{
		for (Polygon_ptr polygon : polygons)
		{
			if (!polygon->isFilled ())
				continue;
			
			polygon->setOwner (m_steps->getCurrentOwner ());
			m_polygons.push_back (polygon);
			
			Point prevPoint = polygon->back ();
			
			for (const Point& currPoint : *polygon.get ())
			{
				m_graph->addEdge (prevPoint, currPoint);
				prevPoint = currPoint;
			}
		}
	}
}

#include "include/dottable.moc"
