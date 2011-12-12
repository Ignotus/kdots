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

#ifndef DOTTABLE_H
#define DOTTABLE_H
#include <QObject>
#include <QPair>
#include <QPoint>
#include <QHash>
#include <memory>

#include "stepqueue.h"


struct Point
{
	Point (const bool owner = false);
	
	const bool Owner_;
	bool Border_;
	bool Captured_;
};

class Polygon;

static int Width_;

class DotTable : public QObject
{
	Q_OBJECT
	
	const int Height_;

	QHash<QPoint, Point> PointHash_;
	StepQueue *StepQueue_;
	
	QList<QPoint> Polygon_;
	QHash<QPoint, bool> TempHash_;
public:
	DotTable (int height, int width, GameMode mode = DEFAULT,
			bool firstPlayer = 0, QObject *parent = 0);
	virtual ~DotTable ();
	
	Point getPoint (int x, int y) const;
	Point getPoint (const QPoint& point) const;
	
	void setPoint (int x, int y);
	void setPoint (const QPoint& point);
private:
	void findPolygon (const QPoint& firstPoint, QList<Polygon>& polygonList);
signals:
	void draw (const Polygon& polygon);
};

inline uint qHash (const QPoint& key)
{
	return (key.y () - 1) * Width_ + key.x ();
}

#endif // DOTTABLE_H
