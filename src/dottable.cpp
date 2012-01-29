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
#include <QDebug>

using namespace std;

DotTable::DotTable (int width, int height, GameMode mode, Owner owner,
		QObject *parent)
: QObject (parent)
, table (width, height)
, tempBuffer (width, vector<bool> (height))
, stepQueue (new StepQueue (mode, owner))
{
}

void DotTable::pushPoint (const IntPoint& point)
{
	if (table[point.x ()][point.y ()].owner != NoneOwner)
		return;
	
	table[point.x ()][point.y ()].owner = stepQueue->getCurrentOwner ();
	
	stepQueue->addPoint (point);

	clearBuffer ();	

	list<Polygon> polygons;
	findPolygon (point, polygons);
	list<IntPoint> capturedPoints;

	const list<IntPoint>& pointList = stepQueue->getOtherPointList ();

	for (Polygon& polygon : polygons)
	{
		for (const IntPoint& point : pointList)
		{
			qDebug () << "POINTS: " << point.x () << point.y ();
			if (!table[point.x ()][point.y ()].captured
					&& polygon.contains (point))
			{
				polygon.setHasPoint (true);
				qDebug () << point.x () << point.y ();
				table[point.x ()][point.y ()].captured = true;
				capturedPoints.push_back (point);
			}
		}
		
		if (!polygon.getHasPoint ())
		{
			for (const IntPoint& point : capturedPoints)
			{
				if (polygon.contains (point))
				{
					polygon.setHasPoint (true);
					break;
				}
			}
		}
		
		if (polygon.getHasPoint ())
			drawPolygon (polygon, stepQueue->getCurrentOwner ());
	}
	
	stepQueue->addCaptured (capturedPoints.size ());
	
	stepQueue->nextStep (capturedPoints.size ());
}

void DotTable::findPolygon (const IntPoint& point, list<Polygon>& polygons)
{
	const GraphPoint& graphPoint = table[point.x ()][point.y ()];
	if (graphPoint.captured
			|| graphPoint.owner != stepQueue->getCurrentOwner ())
		return;
	
	if (point.x () == pointList.begin ()->x ()
			&& point.y () == pointList.begin ()->y ()
			&& pointList.size () > 3)
	{
		polygons.push_back (Polygon (pointList));
		return;
	}
	
	if (tempBuffer[point.x ()][point.y ()])
		return;
	
	pointList.push_back (point);
	tempBuffer[point.x ()][point.y ()] = true;
	
	static int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
	static int dy[8] = {1, 1, 0, -1, -1, -1, 0, 1};
	
	for (int i = 0; i < 8; ++i)
	{
		int tempx = point.x () + dx[i];
		int tempy = point.y () + dy[i];
		if (tempx < 0 || tempy < 0
				|| tempx >= table.width ()
				|| tempy >= table.height ())
			continue;
		
		findPolygon (IntPoint (tempx, tempy), polygons);
	}
	pointList.pop_back ();
}

void DotTable::drawPolygon (const Polygon& polygon, Owner owner)
{
	const list<IntPoint>& pointList = polygon.getPoints ();
	list<IntPoint>::const_iterator itr = polygon.begin ();
	IntPoint prevPoint = pointList.back ();
	for (; itr != polygon.end (); ++itr)
	{
		table.addEdge (prevPoint, *itr);
		prevPoint = *itr;
	}
}

void DotTable::clearBuffer ()
{
	for (int i = 0; i < table.width (); ++i)
	{
		for (int j = 0; j < table.height (); ++j)
		{
			tempBuffer[i][j] = false;
		}
	}
	
	pointList.clear ();
}

Graph DotTable::getGraph () const
{
	return table;
}

int DotTable::getMarks (Owner owner) const
{
	return stepQueue->getMarks (owner);
}
