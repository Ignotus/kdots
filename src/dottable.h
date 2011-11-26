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

enum Owner
{
	FIRST,
	SECOND,
	NIL
};

struct TablePoint
{
	TablePoint (Owner owner = NIL, bool border = false);
	
	Owner owner;
	bool border;
	bool captured;
};

class Polygon;

class DotTable : public QObject
{
	Q_OBJECT
public:
	enum GameMode
	{
		EXTRA_TURN,
		DEFAULT
	};
private:
	int Height_, Width_;
	TablePoint **Table_;
	Owner Turn_;
	GameMode GameMode_;
	bool Captured_;
	
	QList<QPoint> FirstPlayer_, SecondPlayer_;

public:
	DotTable (int height, int width, GameMode mode = DEFAULT,
			Owner turn = FIRST, QObject* = 0);
	virtual ~DotTable ();
	
	TablePoint getPoint (int x, int y) const;
	void setPoint (int x, int y);
	
	Owner getOwner () const;

private:
	Owner other (Owner owner) const;
	
	void findPolygon (int x, int y, QList<QPoint>& polygon,
			QList<Polygon>& polygonList, bool **map);
};

#endif // DOTTABLE_H
