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
#pragma once
#include <QObject>

#include "gameconfig.hpp"
#include "polygon.hpp"

namespace KDots
{
  class Graph;
  class StepQueue;
  class IBoardView;
  class IRival;
  
  class BoardModel;
  class BoardModelPrivate final : public QObject
  {
    Q_OBJECT
    Q_DECLARE_PUBLIC(BoardModel)
    Q_DISABLE_COPY(BoardModelPrivate)
  private:
    BoardModelPrivate(const GameConfig& config,
                      std::unique_ptr<StepQueue>&& step_queue,
                      BoardModel *parent);
  
  private:
    void drawPolygon(PolyList polygons);
    void continueStep();
    void emitStatus();
  
  private Q_SLOTS:
    void addPoint(const QPoint& point);
    
  private:
    std::unique_ptr<Graph> m_graph;
    std::shared_ptr<StepQueue> m_steps;
    GameConfig m_config;
    std::vector<Polygon_ptr> m_polygons;
    std::unique_ptr<IRival> m_rival;
    std::unique_ptr<IBoardView> m_view;
    
    BoardModel *q_ptr;
  };
}