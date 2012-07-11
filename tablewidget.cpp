/*
 * kdots
 * Copyright (C) 2011-2012 Minh Ngo <nlminhtl@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "tablewidget.hpp"
#include <cmath>
#include <vector>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

namespace KDots
{
  TableWidget::TableWidget (int height, int width, GameMode mode,
                            Owner owner, QWidget *parent)
    : QWidget (parent)
    , m_table (new DotTable (width, height, mode, owner))
    , m_height (height + 1)
    , m_width (width + 1)
  {
    setMinimumSize (400, 400);
  }

  void
  TableWidget::mouseMoveEvent (QMouseEvent *event)
  {
  }

  void
  TableWidget::mousePressEvent (QMouseEvent *event)
  {
    const QRect &rectange = rect ();

    const float cellHeight = rectange.height () / m_height;
    const float cellWidth = rectange.width () / m_width;

    const float cellSize = cellHeight > cellWidth ? cellWidth : cellHeight;
    float tableWidth = cellSize * m_width;
    float tableHeight = cellSize * m_height;

    float dx = (rectange.width () - cellSize * m_width) / 2;
    float dy = (rectange.height () - cellSize * m_height) / 2;

    int x = (event->x () - dx) / cellSize;
    int y = (event->y () - dy) / cellSize;

    const float firstPart = cellSize / 3;
    const float lastPart = cellSize * 2 / 3;

    dx = event->x () - dx - x * cellSize;

    if (dx > firstPart && dx < lastPart)
      {
        return;
      }
    else if (dx < firstPart)
      {
        --x;
      }

    dy = event->y () - dy - y * cellSize;

    if (dy > firstPart && dy < lastPart)
      {
        return;
      }
    else if (dy < firstPart)
      {
        --y;
      }

    //TODO: Fix it
    if (x >= m_width - 1 || x < 0 || y < 0 || y >= m_height - 1)
      {
        return;
      }

    qDebug () << Q_FUNC_INFO << __LINE__;
    m_table->pushPoint (Point (x, y));
    qDebug () << Q_FUNC_INFO << __LINE__;
    update ();
  }

  void
  TableWidget::paintEvent (QPaintEvent *event)
  {
    const QRect &rectange = event->rect ();

    const float cellHeight = rectange.height () / m_height;
    const float cellWidth = rectange.width () / m_width;

    const float cellSize = cellHeight > cellWidth ? cellWidth : cellHeight;
    const float tableWidth = cellSize * m_width;
    const float tableHeight = cellSize * m_height;

    QPixmap pixmap (QSize (tableWidth, tableHeight));

    pixmap.fill (Qt::white);

    QPainter pixPainter (&pixmap);
    pixPainter.setRenderHint (QPainter::Antialiasing);

    QPen pen (Qt::black, 1);

    pixPainter.setPen (pen);

    int i, k;

    for (i = cellSize, k = m_width * cellSize; i < k; i += cellSize)
      {
        pixPainter.drawLine (i, 0, i, pixmap.height ());
      }

    for (i = cellSize, k = m_height * cellSize; i < k; i += cellSize)
      {
        pixPainter.drawLine (0, i, pixmap.width (), i);
      }

    const Graph &graph = m_table->graph ();

    const QBrush firstBrush (Qt::red);
    const QBrush secondBrush (Qt::blue);

    const QPen firstPen (Qt::red, 2);
    const QPen secondPen (Qt::blue, 2);

    int j;

    for (i = 0; i < graph.width (); ++i)
      {
        for (k = 0; k < graph.height (); ++k)
          {
            const GraphPoint &point = graph[i][k];

            if (point.owner () == NONE)
              {
                continue;
              }

            pixPainter.setPen (point.owner () == FIRST
                ? firstPen
                : secondPen);

            pixPainter.setBrush (point.owner () == FIRST
                ? firstBrush
                : secondBrush);

            pixPainter.drawEllipse (QPoint ((i + 1) * cellSize,
                                            (k + 1) * cellSize),
                                    3, 3);

            const GraphPoint::GraphEdges& edges = point.edges ();

            for (j = 0; j < edges.size (); ++j)
              {
                const Point& lastPoint = edges[j];

                pixPainter.drawLine ( (i + 1) * cellSize,
                                      (k + 1) * cellSize,
                                      (lastPoint.x () + 1) * cellSize,
                                      (lastPoint.y () + 1) * cellSize);
              }
          }
      }

    const int dx = (rectange.width () - tableWidth) / 2;

    const int dy = (rectange.height () - tableHeight) / 2;

    QPainter painter (this);

    painter.drawPixmap (dx, dy, pixmap);

    emit updateStatusBar (QString ("First:\t")
                          + QString::number (m_table->stepQueue()->getMarks (FIRST))
                          + "\tSecond:\t"
                          + QString::number (m_table->stepQueue()->getMarks (SECOND)));
  }
}