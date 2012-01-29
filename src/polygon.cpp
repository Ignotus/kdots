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
#include "polygon.h"
#include <QDebug>
using namespace std;

Polygon::Polygon (const list<IntPoint>& polygonPoints)
: polygonPoints (polygonPoints)
, hasPoint (false)
{
}

list<IntPoint> Polygon::getPoints () const
{
	return polygonPoints;
}

bool Polygon::contains (const IntPoint& point) const
{
	int i = 0;
	
	list<IntPoint>::const_iterator itr = polygonPoints.begin ();
	list<IntPoint>::const_iterator itrEnd = polygonPoints.end ();
	qDebug () << "Size: " << polygonPoints.size ();
	while (itr != itrEnd)
	{
		if (itr->y () != point.y ())
		{
			++itr;
			continue;
		}
		
		const IntPoint& prevPoint = [&itr, this] ()
		{
			return itr-- == polygonPoints.begin ()
					? polygonPoints.back ()
					: *itr;
		} ();
		++itr;
		
		const IntPoint& nextPoint = [&itr, this] ()
		{
			return ++itr == polygonPoints.end ()
					? polygonPoints.front ()
					: *itr;
		} ();
		--itr;
		
		if (prevPoint.y () != nextPoint.y () && itr->x () < point.x ())
			++i;
		++itr;
	}
	return i % 2;
}

void Polygon::setHasPoint (bool val)
{
	hasPoint = val;
}

bool Polygon::getHasPoint () const
{
	return hasPoint;
}

Polygon::iterator Polygon::begin ()
{
	return polygonPoints.begin ();
}

Polygon::const_iterator Polygon::begin () const
{
	return polygonPoints.begin ();
}

Polygon::iterator Polygon::end ()
{
	return polygonPoints.end ();
}

Polygon::const_iterator Polygon::end () const
{
	return polygonPoints.end ();
}