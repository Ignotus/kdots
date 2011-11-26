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

#include "dottable.h"
#include "polygon.h"
#include <algorithm>
#include <functional>

using namespace std;

TablePoint::TablePoint (Owner owner, bool border)
: owner (owner)
, border (border)
, captured (false)
{
}

DotTable::DotTable (int height, int width, GameMode mode, Owner turn,
		QObject *parent)
: QObject (parent)
, Height_ (height)
, Width_ (width)
, Table_ (new TablePoint* [width])
, Turn_ (turn)
, GameMode_ (mode)
, Captured_ (false)
{
	for (int i = 0; i < width; ++i)
		Table_ [i] = new TablePoint [height];
}

DotTable::~DotTable ()
{
	for (int i = 0; i < Width_; ++i)
		delete [] Table_ [i];
	delete [] Table_;
}

TablePoint DotTable::getPoint (int x, int y) const
{
	return Table_ [x][y];
}

void DotTable::setPoint (int x, int y)
{
	if (Table_ [x][y].owner != NIL)
		return;
	
	Table_ [x][y].owner = Turn_;

	QList<QPoint>& currentPlayer = Turn_ == FIRST ? FirstPlayer_ : SecondPlayer_;
	QList<QPoint>& otherPlayer = Turn_ == FIRST ? SecondPlayer_ : FirstPlayer_;
	currentPlayer << QPoint (x, y);
	
	bool **map = new bool* [Width_];
	for (int i = 0; i < Width_; ++i)
		map [i] = new bool [Height_];
	
	QList<QPoint> polygon;
	QList<Polygon> polygonList;

	// O(n)
	Q_FOREACH (const QPoint& point, polygon)
		map[point.x ()][point.y ()] = true;
	
	findPolygon (x, y, polygon, polygonList, map);
	
	for (int i = 0; i < Width_; ++i)
		delete [] map [i];
	delete [] map;
	
	QList<Polygon>::const_iterator itr = polygonList.begin ();
	for (; itr != polygonList.end (); ++itr)
	{
		remove_if (polygonList.begin (), polygonList.end (), [&itr] (const Polygon& polygon) {
			return itr->contains (polygon) && *itr != polygon;
		});
	}	
	
	Turn_ = getOwner ();
}

const int dx [] = {-1, 0, 1, 1, 1, 0, -1, -1};
const int dy [] = {-1, -1, -1, 0, 1, 1, 1, 0};

void DotTable::findPolygon (int x, int y, QList<QPoint>& polygon,
		QList<Polygon>& polygonList, bool **map)
{
	polygon << QPoint (x, y);
	map[x][y] = false;
	
	if (polygon.size () > 4 && polygon.first () == polygon.last ())
	{
		polygonList << Polygon (polygon);
		polygon.pop_back ();
		return;
	}
	
	for (int i = 0; i < 8; ++i)
	{
		int new_x = x + dx [i];
		int new_y = y + dy [i];
		
		if (new_x < 0 || new_y < 0 || new_x >= Width_ || new_y >= Height_
				|| map[new_x][new_y] || Table_ [new_x][new_y].owner != Turn_)
			continue;
		
		findPolygon (new_x, new_y, polygon, polygonList, map);
	}
	
	polygon.pop_back ();
}

Owner DotTable::other (Owner owner) const
{
	return (owner == FIRST) ? SECOND : FIRST;
}

Owner DotTable::getOwner () const
{
	return Captured_ ? (GameMode_ == EXTRA_TURN ? Turn_ : other (Turn_)) : other (Turn_);
}