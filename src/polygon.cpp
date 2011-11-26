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

Polygon::Polygon (const QList<QPoint>& coordList)
: CoordList_ (coordList)
{
}

bool Polygon::operator== (const Polygon& other) const
{
	return other.CoordList_ == CoordList_;
}

bool Polygon::operator!= (const Polygon & other) const
{
	return other.CoordList_ != CoordList_;
}

// O(n)
bool Polygon::contains (const Polygon& other) const
{	
	return contains (other.CoordList_.first ());
}

bool Polygon::contains (const QPoint& point) const
{
	int count = 0;
	QList<QPoint>::const_iterator itr = CoordList_.begin ();
	for (int itr_y; itr != CoordList_.end (); ++itr, itr_y = itr->y ())
	{
		if (itr_y == point.y ())
		{
			if (itr->x () < point.x ())
			{
				const QPoint& prev = prevPoint (itr, CoordList_);
				const QPoint& next = nextPoint (itr, CoordList_);
				if (!(prev.y () < itr_y && next.y () < itr_y))
					++count;
			}
			else if (itr->x () == point.x ())
				++count;
		}
	}
	
	return !(count % 2);
}

QPoint Polygon::prevPoint (QList<QPoint>::const_iterator itr,
		const QList<QPoint>& points) const
{
	return itr == points.begin () ? points.last () : *(itr - 1);
}

QPoint Polygon::nextPoint (QList<QPoint>::const_iterator itr,
		const QList<QPoint>& points) const
{
	return ++itr == points.end () ? points.first () : *itr;
}