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

Point::Point (const bool owner)
: Owner_ (owner)
, Border_ (false)
, Captured_ (false)
{
}

DotTable::DotTable (int height, int width, GameMode mode, bool firstPlayer,
		QObject *parent)
: QObject (parent)
, Height_ (height)
, StepQueue_ (new StepQueue (mode, firstPlayer))
{
	Width_ = width;
}

DotTable::~DotTable ()
{
	delete StepQueue_;
}

Point DotTable::getPoint (int x, int y) const
{
	return getPoint (QPoint (x, y));
}

Point DotTable::getPoint (const QPoint& point) const
{
	return PointHash_[point];
}

void DotTable::setPoint (int x, int y)
{
	setPoint (QPoint (x, y));
}

void DotTable::setPoint (const QPoint& point)
{
	if (PointHash_.contains (point))
		return;
	
	Polygon_.clear ();
	TempHash_.clear ();
	
	QList<Polygon> polygonList;
	findPolygon (point, polygonList);
	
	//O(n^3)
	QList<Polygon>::const_iterator itr = polygonList.begin ();
	for (; itr != polygonList.end (); ++itr)
	{
		remove_if (polygonList.begin (), polygonList.end (), [&itr] (const Polygon& polygon) {
			return itr->contains (polygon) && *itr != polygon;
		});
	}
	
	bool isCaptured = false;
	
	Q_FOREACH (const Polygon& polygon, polygonList)
	{
		QHashIterator<QPoint, Point> i (PointHash_);
		
		while (i.hasNext ())
		{
			i.next ();
			if (i.value ().Owner_ == StepQueue_->getCurrentOwner ()
					|| !i.value ().Captured_)
				continue;
			
			const QPoint& k = i.key ();
		
			if (polygon.contains (k))
			{
				isCaptured = true;
				emit draw (polygon);
				break;
			}
		}
	}
	
	StepQueue_->nextStep (isCaptured);
}

const int dx[] = {-1, 0, 1, 1, 1, 0, -1, -1};
const int dy[] = {-1, -1, -1, 0, 1, 1, 1, 0};

void DotTable::findPolygon (const QPoint& firstPoint, QList<Polygon>& polygonList)
{
	Polygon_ << QPoint (firstPoint);
	TempHash_[firstPoint] = true;
	
	if (Polygon_.size () > 4 && Polygon_.first () == Polygon_.last ())
	{
		Polygon_.pop_back ();
		polygonList << Polygon (Polygon_);
		return;
	}
	
	for (int i = 0; i < 8; ++i)
	{
		const QPoint& newPoint = QPoint (firstPoint.x () + dx[i],
				firstPoint.y () + dy[i]);
		
		if (newPoint.x () < 0 || newPoint.x () >= Width_
				|| newPoint.y () < 0 || newPoint.x () >= Height_
				|| (TempHash_.contains (newPoint) && TempHash_[newPoint])
				|| (PointHash_.contains (newPoint) && PointHash_[newPoint].Owner_ != StepQueue_->getCurrentOwner ()))
			continue;
		
		findPolygon (newPoint, polygonList);
	}
	
	Polygon_.pop_back ();
}

