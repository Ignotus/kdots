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
 *(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "rival.hpp"
#include <ctime>
#include <iostream>
#include <algorithm>
#include <stack>
#include <QStatusBar>
#include <QLabel>
#include <KgDifficulty>
#include <KDebug>
#include <point.hpp>
#include <boardmodel.hpp>
#include <stepqueue.hpp>
#include <constants.hpp>
#include <polygonfinder.hpp>
#include <graph.hpp>
#include "prioritymap.hpp"

namespace KDots
{
  namespace simpleai
  {
    Rival::Rival(QObject *parent)
      : IRival(parent)
      , m_board(NULL)
      , m_current(Owner::FIRST)
      , m_other(Owner::SECOND)
      , m_iterations(1)
    {
      PriorityMap::instance();
      Kg::difficulty()->setEditable(true);
    }
    
    Owner Rival::owner() const
    {
      return m_current;
    }
    
    bool Rival::hasMask(const Graph& graph, const Point& point, const MapData& mask, const Owner current)
    {
      const MapType& map = mask.m_map;
      const Point& currentPoint = mask.m_current;
      const Owner other = StepQueue::other(current);
      
      for (std::size_t j = 0, height = map.size(), i, width = map.front().size(); j < height; ++j)
      {
        for (i = 0; i < width; ++i)
        {
          const Point& newPoint = point - currentPoint + Point(i, j);
        
          if (!graph.isValid(newPoint))
            return false;
          
          const MapElement el = map[j][i];
          const GraphPoint& graphPoint = graph[newPoint];
          const Owner own = graphPoint.owner();
          
          if (graphPoint.isCaptured() && el != MapElement::NM)
            return false;
          
          switch (el)
          {
          case MapElement::EM: //Empty
            if (own != Owner::NONE)
              return false;
            break;
          case MapElement::FI: //First
            if (own != other)
              return false;
            break;
          case MapElement::SE: //Second
            if (own != current)
              return false;
            break;
          case MapElement::PF: // Possibly first
            if (own == current)
              return false;
            break;
          case MapElement::PS: // Possibly second
            if (own == other)
              return false;
            break;
          case MapElement::NM: case MapElement::CU: default:
            break;
          }
        }
      }
      
      return true;
    }
    
    float Rival::calcPriority(const Point& point)
    {
      float priority = 2;
      const Graph& graph = m_board->graph();
      
      if (m_iterations > 1 && hasCaptured(point, m_current))
        return 1.0;
      
      if (m_iterations > 2)
      {
        bool captured = hasCaptured(point, m_other);
        if (captured)
        {
          for (int i = 0; i < DIRECTION_COUNT; ++i)
          {
            const Point newPoint(point.m_x + GRAPH_DX[i], point.m_y + GRAPH_DY[i]);
            if (!graph.isValid(newPoint))
              continue;
            
            if (graph[newPoint].owner() != Owner::NONE)
              continue;
            
            if (hasCaptured(newPoint, m_other))
            {
              captured = false;
              break;
            }
          }
        }
        
        if(captured)
          return 0.99;
      }
      
      for (const MapData& table : PriorityMap::instance().priorityMap())
      {
        if (!hasMask(graph, point, table, m_current))
          continue;
        
        if (table.m_priority < priority)
          priority = table.m_priority;
      }
      
      return priority > 1.5 ? 0 : priority;
    }
  
    void Rival::calcRange(int& min_x, int& min_y, int& max_x, int& max_y)
    {
      const Graph& graph = m_board->graph();
      for (int j = 0, max_j = graph.height(), max_i = graph.width(), i; j < max_j; ++j)
      {
        for (i = 0; i < max_i; ++i)
        {
          const GraphPoint& point = graph[Point(i, j)];
          if (point.owner() == m_other)
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
    
    void Rival::onDifficultyChanged(const KgDifficultyLevel *level)
    {
      switch (level->standardLevel())
      {
      case KgDifficultyLevel::Easy:
        m_iterations = 1;
        break;
      case KgDifficultyLevel::Medium:
        m_iterations = 2;
        break;
      default:
        m_iterations = 3;
        break;
      }
    }
    
    bool Rival::hasCaptured(const KDots::Point& point, KDots::Owner current) const
    {
      const Graph& graph = m_board->graph();
      auto steps = m_board->stepQueue();
      PolygonFinder findPolygon(graph, current);

      //O(n)
      const PolyList& polyList = findPolygon(point);

      const auto& otherOwnerPoints = steps.getPoints(m_other);
      for (const Point& p : otherOwnerPoints)
      {
        const GraphPoint& gpoint = graph[p];
        if (gpoint.isCaptured())
          continue;
        
        for (const Polygon_ptr& polygon : polyList)
        {
          if (polygon->contains(p) && gpoint.owner() == m_other)
            return true;
        }
      }
      
      return false;
    }
    
    void Rival::onPointAdded(const Point& point)
    {
      if (m_board->stepQueue().getCurrentOwner() == m_board->stepQueue().firstOwner())
        return;
      
      int min_x = point.m_x - 1, min_y = point.m_y - 1;
      int max_x = point.m_x + 1, max_y = point.m_y + 1;
      calcRange(min_x, min_y, max_x, max_y);
      const Point minPoint(min_x, min_y), maxPoint(max_x, max_y); 
      
      const Graph& graph = m_board->graph();
      
      m_points.clear();
      
      float maxPrio = -2;
      for (Graph::const_iterator itr = graph.begin(), end = graph.end(); itr != end; ++itr)
      {
        if (itr->isCaptured() || itr->owner() != Owner::NONE)
          continue;
        
        const Point& curr = itr.point();
        if (!(curr >= minPoint && curr <= maxPoint))
          continue;
        
        const float prio = calcPriority(curr);
        if (prio > maxPrio)
        {
          m_points.clear();
          maxPrio = prio;
          m_points.push_back(curr);
        }
        else if (prio == maxPrio)
          m_points.push_back(curr);
      }
      
      if (!m_points.empty())
      {
        std::srand(std::time(NULL));
        emit needAddPoint(m_points[std::rand() % m_points.size()]);
      }
    }
    
    void Rival::setBoardModel(BoardModel *board)
    {
      m_board = board;
      
      m_other = m_board->stepQueue().getCurrentOwner();
      m_current = StepQueue::other(m_other);
    }
  }
}
