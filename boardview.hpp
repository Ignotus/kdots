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
#include <QWidget>

#include <constants.hpp>
#include <iboardview.hpp>
#include <point.hpp>

class QPainter;

namespace KDots
{
  class IRival;
  class BoardModel;
  class GameConfig;

  class BoardView : public QWidget
                  , public IBoardView
  {
    Q_OBJECT
  public:
    BoardView(QWidget *parent = 0);
    
    void setModel(BoardModel *table);
    
    QObject* getObject();
    
  public Q_SLOTS:
    void onFreezeView(bool freeze);

  protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

  private:
    void calculatePoint(Point& point, QMouseEvent *event);
    void drawPolygons(QPainter& painter, float cellSize);
    void fillPolygon(QPainter& painter, float cellSize);
    void drawLastPoint(QPainter& painter, float cellSize);
    void drawUnderMousePoint(QPainter& painter, float cellSize);
  
  Q_SIGNALS:
    void pointClicked(const Point& point);

  private:
    BoardModel *m_model;
    
    int m_height;
    int m_width;
    
    Point m_underMousePoint;
  };
}