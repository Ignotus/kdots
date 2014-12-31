/*
 * KDots
 * Copyright(c) 2011-2012 Minh Ngo <nlminhtl@gmail.com>
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
#include <QObject>
#include "gameconfig.hpp"
#include "polygon.hpp"

namespace KDots
{
  class Graph;
  class StepQueue;
  class IBoardView;
  class IRival;
  class KDOTS_EXPORT BoardModel : public QObject
  {
    Q_OBJECT
  public:
    BoardModel(const GameConfig& config, std::unique_ptr<StepQueue>&& step_queue, QObject *parent = 0);
    
    void setView(std::unique_ptr<IBoardView>&& view);
    void setRival(std::unique_ptr<IRival>&& rival);
    
    const GameConfig& gameConfig() const;
    
    const std::vector<Polygon_ptr>& polygons() const;

    const Graph& graph() const;

    const StepQueue& stepQueue() const;
    
  public slots:
    void undo();

  private:
    void drawPolygon(PolyList polygons);
    void continueStep();
    void emitStatus();
    
  private slots:
    void addPoint(const Point& point);
    
  signals:
    void pointAdded(const Point& lastPoint);
    void freezeView(bool);
    void statusUpdated(const QString& message);
    
  private:
    std::unique_ptr<Graph> m_graph;
    std::shared_ptr<StepQueue> m_steps;
    GameConfig m_config;
    std::vector<Polygon_ptr> m_polygons;
    std::unique_ptr<IRival> m_rival;
    std::unique_ptr<IBoardView> m_view;
  };
}