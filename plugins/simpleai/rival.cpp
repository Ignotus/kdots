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
#include <KDebug>
#include <include/point.hpp>
#include <include/dottable.hpp>
#include <include/stepqueue.hpp>
#include "prioritymap.hpp"

namespace KDots
{
	namespace simpleai
	{
		Rival::Rival (QObject *parent)
			: IRival (parent)
			, m_table (NULL)
		{
			PriorityMap::instance ();
		}
		
		bool Rival::isAllow () const
		{
			if (!m_table)
				return false;
			
			return m_table->stepQueue ()->getCurrentOwner () == m_table->stepQueue ()->firstOwner ();
		}
		
		namespace
		{
			float calcImportance(const Graph& graph, const Point& point, const Owner currentOwner)
			{
				float priority = -1;
				const Owner otherOwner = StepQueue::other (currentOwner);
				
				for (const MapData& table : PriorityMap::instance ().priorityMap ())
				{
					const MapType& map = table.m_map;
					const Point& currentPoint = table.m_current;
					
					for (std::size_t j = 0, height = map.size (), i,
							width = map.front ().size (); j < height; ++j)
					{
						for (i = 0; i < width; ++i)
						{
							const int dx = currentPoint.x () - i;
							const int dy = currentPoint.y () - j;
							const int newX = point.x () + dx;
							const int newY = point.y () + dy;
							if (newX < 0 || newY < 0 || newX >= graph.width ()
									|| newY >= graph.height ())
								goto endloop;
							
							const MapElement el = map[j][i];
							const GraphPoint& graphPoint = graph[newX][newY];
							const Owner own = graphPoint.owner ();
							switch (el)
							{
							case EM: //Empty
								if (own != NONE)
									goto endloop;
								break;
							case FI: //First
								if (own != currentOwner)
									goto endloop;
								break;
							case SE: //Second
								if (own != otherOwner)
									goto endloop;
								break;
							case PF: // Possibly first
								if (own == otherOwner)
									goto endloop;
								break;
							case PS: // Possibly second
								if (own == currentOwner)
									goto endloop;
							default:
								break;
							}
						}
					}
					
					if (table.m_priority > priority)
						priority = table.m_priority;
endloop:
					;
				}
				
				return priority;
			}
		}
		
		void Rival::nextStep (const Point& point)
		{
			if (isAllow ())
				return;
			
			const Graph& gr = m_table->graph ();
			
			const Owner current = m_table->stepQueue ()->getCurrentOwner ();
			const Owner other = StepQueue::other (current);
			std::vector<Point> points;
			float max_priority = -1;
			
			int min_x = point.x () - 2, min_y = point.y () - 2;
			int max_x = point.x () + 2, max_y = point.y () + 2;
			for (int j = 0, max_j = gr.height (), max_i = gr.width (), i; j < max_j; ++j)
			{
				for (i = 0; i < max_i; ++i)
				{
					const GraphPoint& point = gr[i][j];
					if (point.owner () == other)
					{
						if (i - 2 < min_x)
							min_x = i - 2;
						else if (i + 2 > max_x)
							max_x = i + 2;
						
						if (j - 2 < min_y)
							min_y = j - 2;
						else if (j + 2 > max_y)
							max_y = j + 2;
					}
				}
			}
			
			for (Graph::const_iterator itr = gr.begin (), itrEnd = gr.end ();
					itr != itrEnd; ++itr)
			{
				if (itr->owner () != NONE)
					continue;
					
				const Point& newPoint = itr.point ();
				
				if (newPoint.x () < min_x || newPoint.x () > max_x
						|| newPoint.y () < min_y || newPoint.y () > max_y)
					continue;
				
				const float imp = calcImportance (gr, newPoint, current);
				if (imp == max_priority)
					points.push_back (newPoint);
				else if (imp > max_priority)
				{
					max_priority = imp;
					points.clear ();
					points.push_back (newPoint);
				}
			}
			
			srand (time (NULL));
			if (!points.empty ())
				m_table->pushPoint (points[rand () % points.size ()]);
		}
		
		void Rival::setDotTable (DotTable *table)
		{
			m_table = table;
		}
	}
}
