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

#ifndef POLYGON_H
#define POLYGON_H

#include <QPoint>
#include <QList>

class Polygon
{
	QList<QPoint> CoordList_;
public:
	Polygon (const QList<QPoint>& coordList);
	
	bool contains (const Polygon& other) const;
	bool contains (const QPoint& point) const;
	
	bool operator== (const Polygon& other) const;
	bool operator!= (const Polygon& other) const;
private:
	QPoint prevPoint (QList<QPoint>::const_iterator itr,
			const QList<QPoint>& points) const;
	QPoint nextPoint (QList<QPoint>::const_iterator itr,
			const QList<QPoint>& points) const;
};

#endif // POLYGON_H
