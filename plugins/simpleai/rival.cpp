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
#include "rival.hpp"
#include "prioritymap.hpp"

#include <boardmodel.hpp>
#include <constants.hpp>
#include <graph.hpp>
#include <point.hpp>
#include <stepqueue.hpp>

#include <map>

#include <KDebug>
#include <KgDifficulty>

namespace KDots
{
  namespace simpleai
  {
    const std::map<KgDifficultyLevel::StandardLevel, int> DIFFICULTY_TO_DEPTH = {
      {KgDifficultyLevel::Easy, 1},
      {KgDifficultyLevel::Medium, 2},
      {KgDifficultyLevel::Hard, 3}
    };
    
    Rival::Rival(QObject *parent)
      : IRival(parent)
      , m_board(nullptr)
      , m_human(Owner::FIRST)
      , m_ai(Owner::SECOND)
      , m_depth(3)
    {
      PriorityMap::instance();
      Kg::difficulty()->setEditable(false);
    }
    
    Owner Rival::owner() const
    {
      return m_ai;
    }
    
    void Rival::onDifficultyChanged(const KgDifficultyLevel *level)
    {
      m_depth = DIFFICULTY_TO_DEPTH.at(level->standardLevel());
    }
    
    void Rival::onPointAdded(const Point& point)
    {
      if (m_board->stepQueue().getCurrentOwner() == m_human)
        return;
      
      addPoint();
    }
    
    void Rival::addPoint()
    {
      const Graph& graph = m_board->graph();
      
      for (std::size_t x = 0; x < graph.width(); ++x)
      {
        for (std::size_t y = 0; y < graph.width(); ++y)
        {
          if (graph[x][y].owner() == Owner::NONE)
          {
            kDebug() << "AddPoint" << x << y;
            emit needAddPoint(Point(x, y));
            goto addPoint_end;
          }
        }
      }
addPoint_end:
        ;
    }
    
    void Rival::setBoardModel(BoardModel *board)
    {
      m_board = board;
      
      if (m_board->stepQueue().getCurrentOwner() == m_ai)
      {
        addPoint();
      }
    }
  }
}
