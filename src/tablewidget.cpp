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
#include <QMouseEvent>
#include <QPaintEvent>
#include "polygon.h"
#include <QPainter>
#include <QDebug>

TableWidget::TableWidget (int height, int width, GameMode mode, bool firstPlayer,
		QWidget *parent)
: QWidget (parent)
, DotTable_ (new DotTable (height, width, mode, firstPlayer, parent))
, Pixmap_ (new QPixmap (QSize ((width + 1) * 40, (height + 1) * 40)))
, Height_ (height)
, Width_ (width)
{
	connect (DotTable_,
			SIGNAL (draw (Polygon)),
			this,
			SLOT (drawPolygon (Polygon)));
	
	
	Pixmap_->fill (Qt::white);
	QPainter painter (Pixmap_);
	
	QPen linePen (QColor (Qt::black));
	linePen.setWidth (1);
	painter.setPen (linePen);
	int i, k;
	for (i = 40, k = (width + 1) * 40; i < k; i += 40)
		painter.drawLine (i, 0, i, Pixmap_->height ());
	
	for (i = 40, k = (height + 1) * 40; i < k; i += 40)
		painter.drawLine (0, i , Pixmap_->width (), i);

	//drawPixmap ();
	//painter.drawPixmap (Pixmap_->copy (rect ()));
}

void TableWidget::drawPixmap ()
{
}

void TableWidget::mousePressEvent (QMouseEvent *event)
{
	
}

void TableWidget::paintEvent (QPaintEvent *event)
{
	const QRect& rectange = event->rect ();
	
	QPainter painter (this);
	painter.setRenderHint (QPainter::HighQualityAntialiasing);
	float cellHeight = (float) rectange.height () / Height_;
	float cellWidth = (float) rectange.width () / Width_;
	
	float cellSize = cellHeight > cellWidth ? cellWidth : cellHeight;
	float tableWidth = cellSize * Width_;
	float tableHeight = cellSize * Height_;
	
	float x = cellHeight > cellWidth ? 0 : (float) (rectange.width () - tableWidth) / 2;
	float y = cellHeight > cellWidth ? (float) (rectange.height () - tableHeight) / 2 : 0;
	
	painter.drawPixmap(QPointF (x, y), Pixmap_->scaled (tableWidth, tableHeight));
}

TableWidget::~TableWidget ()
{
	delete DotTable_;
}

void TableWidget::drawPolygon (const Polygon& polygon)
{
}

void TableWidget::drawPoint (const QPoint& point)
{
	
}