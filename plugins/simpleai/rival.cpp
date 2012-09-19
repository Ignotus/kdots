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
#include "rival.hpp"
#include <ctime>
#include <iostream>
#include <algorithm>
#include <stack>
#include <QStatusBar>
#include <QLabel>
#ifdef NEW_LIBKDEGAMES
# include <KgDifficulty>
#else
# include <KGameDifficulty>
#endif
#include <KDebug>
#include <point.hpp>
#include <dottable.hpp>
#include <stepqueue.hpp>
#include <constants.hpp>
#include <polygonfinder.hpp>
#include <graph.hpp>
#include "prioritymap.hpp"

namespace KDots
{
	namespace simpleai
	{
		Rival::Rival (QObject *parent)
			: IRival (parent)
			, m_table (NULL)
			, m_current (FIRST)
			, m_other (SECOND)
			, m_iterations (1)
		{
			PriorityMap::instance ();
#ifdef NEW_LIBKDEGAMES
				Kg::difficulty ()->setEditable (true);
#else
				KGameDifficulty::setEnabled (true);
#endif
		}
		
		bool Rival::isAllow () const
		{
			if (!m_table)
				return false;
			
			return m_table->stepQueue ()->getCurrentOwner () == m_table->stepQueue ()->firstOwner ();
		}
		
		bool Rival::hasMask (const Graph& graph, const Point& point, const MapData& mask, const Owner current)
		{
			const MapType& map = mask.m_map;
			const Point& currentPoint = mask.m_current;
			const Owner other = StepQueue::other (current);
			
			for (std::size_t j = 0, height = map.size (), i, width = map.front ().size (); j < height; ++j)
			{
				for (i = 0; i < width; ++i)
				{
					const Point& newPoint = point - currentPoint + Point (i, j);
				
					if (!graph.isValid (newPoint))
						return false;
					
					const MapElement el = map[j][i];
					const GraphPoint& graphPoint = graph[newPoint];
					const Owner own = graphPoint.owner ();
					
					if (graphPoint.isCaptured () && el != NM)
						return false;
					
					switch (el)
					{
					case EM: //Empty
						if (own != NONE)
							return false;
						break;
					case FI: //First
						if (own != other)
							return false;
						break;
					case SE: //Second
						if (own != current)
							return false;
						break;
					case PF: // Possibly first
						if (own == current)
							return false;
						break;
					case PS: // Possibly second
						if (own == other)
							return false;
						break;
					case NM: case CU: default:
						break;
					}
				}
			}
			
			return true;
		}
		
		float Rival::calcPriority (const Point& point)
		{
			float priority = 2;
			const Graph& graph = m_table->graph ();
			
			if (m_iterations > 1 && hasCaptured (point, m_current))
				return 1.0;
			
			if (m_iterations > 2)
			{
				bool captured = hasCaptured (point, m_other);
				if (captured)
				{
					for (int i = 0; i < DIRECTION_COUNT; ++i)
					{
						const Point newPoint (point.x () + GRAPH_DX[i], point.y () + GRAPH_DY[i]);
						if (!graph.isValid (newPoint))
							continue;
						
						if (graph[newPoint].owner () != NONE)
							continue;
						
						if (hasCaptured (newPoint, m_other))
						{
							captured = false;
							break;
						}
					}
				}
				
				if (captured)
					return 0.99;
			}
			
			for (const MapData& table : PriorityMap::instance ().priorityMap ())
			{
				if (!hasMask (graph, point, table, m_current))
					continue;
				
				if (table.m_priority < priority)
					priority = table.m_priority;
			}
			
			return priority > 1.5 ? 0 : priority;
		}
	
		namespace
		{
			bool isEmptyAround (const Graph& graph, const Point& point)
			{
				for (int i = 0; i < DIRECTION_COUNT; ++i)
				{
					const Point newPoint (point.x () + GRAPH_DX[i], point.y () + GRAPH_DY[i]);
					if (!graph.isValid (newPoint))
						continue;
					
					if (graph[newPoint].owner () != NONE)
						return false;
				}
				
				return true;
			}
		}
		
		void Rival::calcRange (int& min_x, int& min_y, int& max_x, int& max_y)
		{
			const Graph& graph = m_table->graph ();
			for (int j = 0, max_j = graph.height (), max_i = graph.width (), i; j < max_j; ++j)
			{
				for (i = 0; i < max_i; ++i)
				{
					const GraphPoint& point = graph[Point (i, j)];
					if (point.owner () == m_other)
					{
						if (i - 1 < min_x)
							min_x = i - 1;
						else if (i + 1 > max_x)
							max_x = i + 1;
						
						if (j - 1 < min_y)
							min_y = j - 1;
						else if (j + 1 > max_y)
							max_y = j + 1;
					}
				}
			}
		}
		
		bool Rival::hasCaptured (const KDots::Point& point, KDots::Owner current) const
		{
			const Graph& graph = m_table->graph ();
			auto steps = m_table->stepQueue ();
			PolygonFinder findPolygon (graph, current);

			//O(n)
			const PolyList& polyList = findPolygon (point);

			const auto& otherOwnerPoints = steps->getPoints (m_other);
			for (const Point& p : otherOwnerPoints)
			{
				const GraphPoint& gpoint = graph[p];
				if (gpoint.isCaptured ())
					continue;
				
				for (const Polygon_ptr& polygon : polyList)
				{
					if (DotTable::isInPolygon (polygon, p) && gpoint.owner () == m_other)
						return true;
				}
			}
			
			return false;
		}
		
		std::vector<Point> Rival::possibleMoves () const
		{
			return m_points;
		}
		
		void Rival::nextStep (const Point& point)
		{
			if (isAllow ())
				return;
			
			int min_x = point.x () - 1, min_y = point.y () - 1;
			int max_x = point.x () + 1, max_y = point.y () + 1;
			calcRange (min_x, min_y, max_x, max_y);
			const Point minPoint (min_x, min_y), maxPoint (max_x, max_y); 
			
			const Graph& graph = m_table->graph ();
			
			m_points.clear ();
			
			float maxPrio = -2;
			for (Graph::const_iterator itr = graph.begin (), end = graph.end (); itr != end; ++itr)
			{
				if (itr->isCaptured () || itr->owner () != NONE)
					continue;
				
				const Point& curr = itr.point ();
				if (!(curr >= minPoint && curr <= maxPoint))
					continue;
				
				const float prio = calcPriority (curr);
				if (prio > maxPrio)
				{
					m_points.clear ();
					maxPrio = prio;
					m_points.push_back (curr);
				}
				else if (prio == maxPrio)
					m_points.push_back (curr);
			}
			
			if (!m_points.empty ())
			{
				std::srand (std::time (NULL));
				m_table->pushPoint (m_points[std::rand () % m_points.size ()]);
			}
		}
		
		void Rival::setDotTable (DotTable *table)
		{
			m_table = table;
			
			m_other = m_table->stepQueue ()->getCurrentOwner ();
			m_current = StepQueue::other (m_other);
		}
	}
}
