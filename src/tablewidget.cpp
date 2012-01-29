/*
 * kdots
 * Copyright (C) 2011 Minh Ngo <nlminhtl@gmail.com>
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

#include "tablewidget.h"

#include <cmath>
#include <vector>

using namespace std;

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

#include "polygon.h"

TableWidget::TableWidget (int height, int width, GameMode mode,
		Owner owner, QWidget *parent)
: QWidget (parent)
, dotTable (new DotTable (width, height, mode, owner, this))
, height (height + 1)
, width (width + 1)
{
	setMinimumSize (400, 400);
}

void TableWidget::mousePressEvent (QMouseEvent *event)
{
	const QRect& rectange = rect ();
	
	const float cellHeight = rectange.height () / height;
	const float cellWidth = rectange.width () / width;
	
	const float cellSize = cellHeight > cellWidth ? cellWidth : cellHeight;
	float tableWidth = cellSize * width;
	float tableHeight = cellSize * height;
	
	float dx = (rectange.width () - cellSize * width) / 2;
	float dy = (rectange.height () - cellSize * height) / 2;

	int x = (event->x () - dx) / cellSize;
	int y = (event->y () - dy) / cellSize;
	
	const float firstPart = cellSize / 3;
	const float lastPart = cellSize * 2 / 3;
	
	dx = event->x () - dx - x * cellSize;
	if (dx > firstPart && dx < lastPart)
		return;
	else if (dx < firstPart)
		--x;
	
	dy = event->y () - dy - y * cellSize;
	if (dy > firstPart && dy < lastPart)
		return;
	else if (dy < firstPart)
		--y;
	
	dotTable->pushPoint (IntPoint (x, y));
	update ();
}

void TableWidget::paintEvent (QPaintEvent *event)
{
	const QRect& rectange = event->rect ();
	
	const float cellHeight = rectange.height () / height;
	const float cellWidth = rectange.width () / width;
	
	const float cellSize = cellHeight > cellWidth ? cellWidth : cellHeight;
	const float tableWidth = cellSize * width;
	const float tableHeight = cellSize * height;
	
	QPixmap pixmap (QSize (tableWidth, tableHeight));
	
	pixmap.fill (Qt::white);
	
	QPainter pixPainter (&pixmap);
	pixPainter.setRenderHint (QPainter::Antialiasing);
	
	QPen pen (Qt::black, 1);

	pixPainter.setPen (pen);
	
	int i, k;
	for (i = cellSize, k = width * cellSize; i < k; i += cellSize)
		pixPainter.drawLine (i, 0, i, pixmap.height ());
	
	for (i = cellSize, k = height * cellSize; i < k; i += cellSize)
		pixPainter.drawLine (0, i, pixmap.width (), i);
	
	const Graph& graph = dotTable->getGraph ();
	
	const QBrush firstBrush (Qt::red);
	const QBrush secondBrush (Qt::blue);
	
	const QPen firstPen (Qt::red, 2);
	const QPen secondPen (Qt::blue, 2);
	
	int j;
	for (i = 0; i < graph.width (); ++i)
	{
		for (k = 0; k < graph.height (); ++k)
		{
			const GraphPoint& point = graph[i][k];
			if (point.owner == NoneOwner)
				continue;
			
			pixPainter.setPen (point.owner == FirstOwner
					? firstPen
					: secondPen);
			
			pixPainter.setBrush (point.owner == FirstOwner
					? firstBrush
					: secondBrush);
			
			pixPainter.drawEllipse (QPoint ((i + 1) * cellSize, (k + 1) * cellSize),
					3, 3);
			
			const EdgeList<8, int>& edges = point.edges;
			for (j = 0; j < edges.size (); ++j)
			{
				const auto& lastPoint = edges[j];
				
				pixPainter.drawLine ((i + 1) * cellSize,
						(k + 1) * cellSize,
						(lastPoint.x () + 1) * cellSize,
						(lastPoint.y () + 1) * cellSize);
			}
		}
	}
	
	int dx = (rectange.width () - tableWidth) / 2;
	int dy = (rectange.height () - tableHeight) / 2;
	
	QPainter painter (this);
	painter.drawPixmap (dx, dy, pixmap);
	
	emit updateStatusBar (QString ("First:\t")
			+ QString::number (dotTable->getMarks (FirstOwner))
			+ "\tSecond:\t"
			+ QString::number (dotTable->getMarks (SecondOwner)));
}

TableWidget::~TableWidget ()
{
	delete dotTable;
}
