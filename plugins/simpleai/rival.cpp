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
			return true;
		}
		
		namespace
		{
			float calcImportance(const Graph& graph, const Point& point)
			{
				float priority = -1;
				const GraphPoint& thisPoint = graph[point];
				const Owner currentOwner = thisPoint.owner ();
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
							
							const MapElement el = map[i][j];
				
							const GraphPoint& graphPoint = graph[point.x () + dx][point.y () + dy];
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
		}
		
		void Rival::setDotTable (DotTable *table)
		{
			m_table = table;
		}
	}
}
