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
#include <memory>

#include <QObject>

#include "kdots_api.hpp"
#include "polygon.hpp"

namespace KDots
{
  class GameConfig;
  class Graph;
  class StepQueue;
  class IBoardView;
  class IRival;
  
  class BoardModelPrivate;
  class KDOTS_EXPORT BoardModel : public QObject
  {
    Q_OBJECT
    Q_DECLARE_PRIVATE(BoardModel)
    Q_DISABLE_COPY(BoardModel)
  public:
    BoardModel(const GameConfig& config,
               std::unique_ptr<StepQueue>&& step_queue,
               QObject *parent = 0);
    virtual ~BoardModel();
    
    void setView(std::unique_ptr<IBoardView>&& view);
    void setRival(std::unique_ptr<IRival>&& rival);
    
    const GameConfig& gameConfig() const;
    
    const std::vector<Polygon_ptr>& polygons() const;

    const Graph& graph() const;

    const StepQueue& stepQueue() const;
    
  public Q_SLOTS:
    void undo();
    
  Q_SIGNALS:
    void pointAdded(const QPoint& lastPoint);
    void freezeView(bool);
    void statusUpdated(const QString& message);
    void showMessage(const QString& message);
  
  private:
    const QScopedPointer<BoardModelPrivate> d_ptr;
  };
}