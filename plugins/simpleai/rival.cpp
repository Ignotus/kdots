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
#include <algorithm>
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
			, m_iterations (3)
		{
			PriorityMap::instance ();
#ifdef NEW_LIBKDEGAMES
				Kg::difficulty ()->setEditable (true);
#else
				KGameDifficulty::setEnabled (true);
#endif
		}
		
		void Rival::setDifficulty (int diff)
		{
			m_iterations = diff;
		}
		
		Rival::~Rival()
		{
		}
		
		bool Rival::isAllow () const
		{
			if (!m_table)
				return false;
			
			return m_table->stepQueue ()->getCurrentOwner () == m_table->stepQueue ()->firstOwner ();
		}
		
		bool Rival::hasMask (const Point& point, const MapData& mask)
		{
			const Graph& graph = m_table->graph ();
			const Owner currentOwner = m_table->stepQueue ()->getCurrentOwner ();
			const Owner otherOwner = StepQueue::other (currentOwner);
			
			const MapType& map = mask.m_map;
			const Point& currentPoint = mask.m_current;
				
			for (std::size_t j = 0, height = map.size (), i,
					width = map.front ().size (); j < height; ++j)
			{
				for (i = 0; i < width; ++i)
				{
					const Point newPoint (currentPoint.x () - i + point.x (),
							currentPoint.y () - j + point.y ());
				
					if (!graph.isValid (newPoint))
						return false;
					
					const MapElement el = map[j][i];
					const GraphPoint& graphPoint = graph[newPoint];
					const Owner own = graphPoint.owner ();
					const bool captured = graphPoint.isCaptured ();
					
					if (captured)
						return false;

					switch (el)
					{
					case EM: //Empty
						if (own != NONE)
							return false;
						break;
					case FI: //First
						if (own != otherOwner)
							return false;
						break;
					case SE: //Second
						if (own != currentOwner)
							return false;
						break;
					case PF: // Possibly first
						if (own == currentOwner)
							return false;
						break;
					case PS: // Possibly second
						if (own == otherOwner)
							return false;
						break;
					case NM: case CU:
						break;
					default:
						kDebug () << "WTF";
						break;
					}
				}
			}
			
			return true;
		}
		
		float Rival::calcImportance(const Point& point)
		{
				float priority = -0.5;
			
			int id = 0;
			for (const MapData& table : PriorityMap::instance ().priorityMap ())
			{
				if (!hasMask (point, table))
				{
					++id;
					continue;
				}
// 				else
// 				{
// 					kDebug () << "Found mask #" << id << "\n"
// 							<< table.toString ()
// 							<< "in the point {" << point.x () << ", " << point.y () << "}";
// 				}
				
					if (table.m_priority > priority)
						priority = table.m_priority;
				
				++id;
			}
			
			return priority;
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
			
			bool minSize (const Point& lastPoint, const std::vector<Point> points)
			{
				int distance = 10000;
				int id = 0;
				int index = 0;
				for (const Point& point : points)
				{
					const int sqrDistance = Point::sqrLength (point, lastPoint);
					if (sqrDistance < distance)
					{
						distance = sqrDistance;
						index = id;
					}
					
					++id;
				}
				
				return index;
			};
		}
		
		void Rival::calcRange (int& min_x, int& min_y, int& max_x, int& max_y)
		{
			const Graph& graph = m_table->graph ();
			for (int j = 0, max_j = graph.height (), max_i = graph.width (), i; j < max_j; ++j)
			{
				for (i = 0; i < max_i; ++i)
				{
					const GraphPoint& point = graph[Point (i, j)];
					if (point.owner () != NONE)
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
		
		bool Rival::hasCaptured (const Point& point, Owner current)
		{
			const Graph& graph = m_table->graph ();
			auto steps = m_table->stepQueue ();
			PolygonFinder findPolygon (graph, current);

			//O(n)
			const PolyList& polyList = findPolygon (point);

			const Owner otherOwner = StepQueue::other (current);
			
			const auto& otherOwnerPoints = steps->getPoints (otherOwner);
			for (const Point& p : otherOwnerPoints)
			{
				const GraphPoint& gpoint = graph[p];
				if (gpoint.isCaptured ())
					continue;
				
				for (const Polygon_ptr& polygon : polyList)
				{
					if (DotTable::isInPolygon (polygon, p)
							&& gpoint.owner () == otherOwner)
						return true;
				}
			}
			
			return false;
		}
		
		void Rival::nextStep (const Point& point)
		{
			if (isAllow ())
				return;
			
			int min_x = point.x () - 1, min_y = point.y () - 1;
			int max_x = point.x () + 1, max_y = point.y () + 1;
			calcRange (min_x, min_y, max_x, max_y);
			
			m_pointStack.clear ();
			
			std::vector<Point> points;
			float max_priority = -0.5 * m_iterations;
			const Graph& graph = m_table->graph ();
			for (Graph::const_iterator itr = graph.begin (), itrEnd = graph.end ();
					itr != itrEnd; ++itr)
			{
				if (itr->owner () != NONE || itr->isCaptured ())
					continue;
				
				const Point& newPoint = itr.point ();
				
				if (isEmptyAround (graph, newPoint))
					continue;
				
				if (!(newPoint >= Point (min_x, min_y)
						&& newPoint <= Point (max_x, max_y)))
					continue;
				
				float imp = 0;
				calcImportanceTree (imp, newPoint, 1);
				
				if (imp == max_priority)
					points.push_back (newPoint);
				else if (imp > max_priority)
				{
					max_priority = imp;
					kDebug () << "Max priority is changed to" << max_priority;
					points.clear ();
					points.push_back (newPoint);
				}
			}
			
			if (!points.empty ())
			{
				//const int index = minSize (point, points);
				srand (std::time (NULL));
				m_table->pushPoint (points[rand () % points.size ()]);
			}
		}
		
		void Rival::calcImportanceTree (float& importance, const Point& point, int iteration)
		{
			const Owner current = m_table->stepQueue ()->getCurrentOwner ();
			if (hasCaptured (point, current))
				importance += 0.9;
			else
				importance += calcImportance (point);
			
			m_pointStack.push_back (point);
			if (iteration == m_iterations) // Need configure this feature
				return;
			
			float max_imp =  -m_iterations;
			
			int i = 0;
			const Graph& graph = m_table->graph ();
			for (; i < DIRECTION_COUNT; ++i)
			{
				const Point newPoint (point.x () + GRAPH_DX[i], point.y () + GRAPH_DY[i]);
				if (!graph.isValid (newPoint))
					continue;
				
				const GraphPoint& newGrPoint = graph[newPoint];
				
				if (!newGrPoint.isCaptured () && newGrPoint.owner () == NONE
						&& std::find (m_pointStack.begin (), m_pointStack.end (), newPoint) == m_pointStack.end ())
				{
					float imp = 0;
					calcImportanceTree (imp, newPoint, iteration + 1);
					
						if (max_imp == -m_iterations || imp < max_imp)
							max_imp = imp;
				}
			}
			
			if (i != DIRECTION_COUNT)
				importance += max_imp;
		}
		
		void Rival::setDotTable (DotTable *table)
		{
			m_table = table;
			
			m_current = m_table->stepQueue ()->getCurrentOwner ();
			m_other = StepQueue::other (m_current);
		}
	}
}
