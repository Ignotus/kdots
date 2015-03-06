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
#include "RulesViewer.hpp"
#include "RulesModel.hpp"

#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include <QKeyEvent>

RulesViewer::RulesViewer(QWidget *parent)
  : QWidget(parent)
  , m_model(nullptr)
{
}

void RulesViewer::setModel(RulesModel *model)
{
  m_model = model;
}

void RulesViewer::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Left)
  {
    m_model->setCurrentId(m_model->currentId() - 1);
  }
  else if (event->key() == Qt::Key_Right)
  {
    m_model->setCurrentId(m_model->currentId() + 1);
  }
  
  update();
}

void RulesViewer::paintEvent(QPaintEvent* event)
{
  if (m_model == nullptr)
    return;
  
  const QRect& rectange = event->rect();

  QPixmap pixmap(rectange.size());
  pixmap.fill(Qt::white);

  QPainter painter(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing);
  
  const QVector<QVector<QString>>& points = m_model->points();
  
  const float dx = 50.0;
  const float dy = 50.0;
  
  float yc = dy;
  for (int y = 0; y < points.size(); ++y)
  {
    float xc = dx;
    for (int x = 0; x < points[y].size(); ++x, xc += dx)
    {
      const QString& s = points[y][x];
      if (s != "NM")
      {
        if (s == "FI")
          painter.setBrush(QBrush(Qt::darkRed));
        else if (s == "SE")
          painter.setBrush(QBrush(Qt::darkBlue));
        else if (s == "PF")
          painter.setBrush(QBrush(Qt::red));
        else if (s == "PS")
          painter.setBrush(QBrush(Qt::blue));
        else if (s == "CU")
          painter.setBrush(QBrush(Qt::black));
        else
          painter.setBrush(QBrush());
        
        painter.drawEllipse(QPointF(xc, yc), 5, 5);
      }
    }
    
    yc += dy;
  }
  
  painter.drawText(QPointF(dx, yc), QString::number(m_model->currentId() + 1) + " " + QString::number(m_model->importance()));
  
  QPainter(this).drawPixmap(0, 0, pixmap);
}

#include "RulesViewer.moc"