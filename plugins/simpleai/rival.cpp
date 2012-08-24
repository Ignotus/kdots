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
#ifndef KDEGAMES_4_9
# include <KGameDifficulty>
#endif
#include <include/point.hpp>
#include <include/dottable.hpp>
#include <include/stepqueue.hpp>
#include <include/constants.hpp>
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
			, m_statusBar (NULL)
		{
			PriorityMap::instance ();
#ifndef KDEGAMES_4_9
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
		
		namespace
		{
			float calcImportance(const Graph& graph, const Point& point, const Owner currentOwner)
			{
				float priority = -0.5;
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
							const bool captured = graphPoint.isCaptured ();
							switch (el)
							{
							case EM: //Empty
								if (own != NONE || captured)
									goto endloop;
								break;
							case FI: //First
								if (own != otherOwner || captured)
									goto endloop;
								break;
							case SE: //Second
								if (own != currentOwner || captured)
									goto endloop;
								break;
							case PF: // Possibly first
								if (own == currentOwner || captured)
									goto endloop;
								break;
							case PS: // Possibly second
								if (own == otherOwner || captured)
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
			
			bool isEmptyAround (const Graph& graph, const Point& point)
			{
				for (int i = 0; i < 8; ++i)
				{
					const Point newPoint (point.x () + GRAPH_DX[i], point.y () + GRAPH_DY[i]);
					if (newPoint.x () < 0 || newPoint.y () < 0
							|| newPoint.x () >= graph.width () || newPoint.y () >= graph.height ())
						continue;
					
					if (graph[newPoint].owner () != NONE)
						return false;
				}
				
				return true;
			}
		}
		
		void Rival::nextStep (const Point& point)
		{
			if (isAllow ())
				return;
			
			const Graph& gr = m_table->graph ();
			
			std::vector<Point> points;
			float max_priority = -0.5 * m_iterations;
			
			int min_x = point.x () - 1, min_y = point.y () - 1;
			int max_x = point.x () + 1, max_y = point.y () + 1;
			for (int j = 0, max_j = gr.height (), max_i = gr.width (), i; j < max_j; ++j)
			{
				for (i = 0; i < max_i; ++i)
				{
					const GraphPoint& point = gr[i][j];
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
			
			m_pointStack.clear ();
			
			for (Graph::const_iterator itr = gr.begin (), itrEnd = gr.end ();
					itr != itrEnd; ++itr)
			{
				if (itr->owner () != NONE || itr->isCaptured ())
					continue;
				
				const Point& newPoint = itr.point ();
				
				if (isEmptyAround (gr, newPoint))
					continue;
				
				if (newPoint.x () < min_x || newPoint.x () > max_x
						|| newPoint.y () < min_y || newPoint.y () > max_y)
					continue;
				
				float imp = 0;
				
				calcImportanceTree (imp, newPoint, 1);
				
				if (imp == max_priority)
					points.push_back (newPoint);
				else if (imp > max_priority)
				{
					max_priority = imp;
					points.clear ();
					points.push_back (newPoint);
				}
			}
			
			if (!points.empty ())
				m_table->pushPoint (points[rand () % points.size ()]);
		}
		
		void Rival::calcImportanceTree (float& importance, const Point& point, int iteration)
		{
			const Graph& gr = m_table->graph ();

			importance += calcImportance (gr, point, m_current);
			m_pointStack.push_back (point);
			if (iteration == m_iterations) // Need configure this feature
				return;
			
			float max_imp =  -m_iterations;
			
			int i = 0;
			for (; i < 8; ++i)
			{
				const int tempx = point.x () + GRAPH_DX[i];
				const int tempy = point.y () + GRAPH_DY[i];

				if (tempx < 0 || tempy < 0 || tempx >= gr.width () || tempy >= gr.height ())
					continue;
				
				const Point newPoint (tempx, tempy);
				const GraphPoint& newGrPoint = gr[newPoint];
				
				if (!newGrPoint.isCaptured () && newGrPoint.owner () == NONE
						&& std::find (m_pointStack.begin (), m_pointStack.end (), newPoint) == m_pointStack.end ())
				{
					float imp = 0;
					calcImportanceTree (imp, newPoint, iteration + 1);
					
					if (max_imp == -m_iterations || imp < max_imp)
						max_imp = imp;
				}
			}
			
			if (i != 8)
				importance += max_imp;
		}
		
		void Rival::setDotTable (DotTable *table)
		{
			m_table = table;
			
			m_current = m_table->stepQueue ()->getCurrentOwner ();
			m_other = StepQueue::other (m_current);
		}
		
		void Rival::setStatusBar (QStatusBar* bar)
		{
		}

	}
}
