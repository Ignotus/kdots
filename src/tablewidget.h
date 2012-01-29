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

#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QWidget>
#include <QPainter>
#include "dottable.h"

class TableWidget : public QWidget
{
	Q_OBJECT
	
	DotTable *dotTable;
	int height, width;
public:
	TableWidget (int height, int width, GameMode mode,
			Owner owner = FirstOwner, QWidget *parent = 0);
	virtual ~TableWidget ();
	
protected:
	void mousePressEvent (QMouseEvent *event);
	void paintEvent (QPaintEvent *event);
};

#endif // TABLEWIDGET_H
