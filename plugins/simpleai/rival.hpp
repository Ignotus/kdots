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
#pragma once
#include <memory>
#include <QLabel>
#include <point.hpp>
#include <interface/irival.hpp>

namespace KDots
{
  class Graph;
  namespace simpleai
  {
    struct MapData;
    
    class Rival : public KDots::IRival
    {
      Q_OBJECT
      Q_INTERFACES(KDots::IRival)
      
      BoardModel *m_board;
      Owner m_current, m_other;
      int m_iterations;
      std::vector<Point> m_points;
    public:
      Rival(QObject *parent = 0);
      
      Owner owner() const;
      
      static bool hasMask(const Graph& graph, const Point& point, const MapData& mask, const Owner current);
      
      void setBoardModel(BoardModel *board);
      
    public slots:
      void onPointAdded(const Point& point);
      void onDifficultyChanged(const KgDifficultyLevel *level);
      
    private:
      float calcPriority(const Point& point);
      void calcRange(int& min_x, int& min_y, int& max_x, int& max_y);
      bool hasCaptured(const Point& point, Owner current) const;
      
    signals:
      void needCreateBoard(const GameConfig& config);
      void needDestroy();
      void needAddPoint(const Point&);
    };
  }
}
