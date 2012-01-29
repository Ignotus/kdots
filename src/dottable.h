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

#ifndef KDOTS_DOTTABLE_H
#define KDOTS_DOTTABLE_H

#include <memory>
#include <vector>

#include <QObject>

#include "stepqueue.h"
#include "polygon.h"
#include "graph.h"

class DotTable : public QObject
{
	Q_OBJECT
	
	Graph table;
	std::vector< std::vector<bool> > tempBuffer;
	std::list<IntPoint> pointList;
	
	std::shared_ptr<StepQueue> stepQueue;
public:
	DotTable (int width, int height, GameMode mode, Owner owner,
			QObject *parent);
	
	void pushPoint (const IntPoint& point);
	
	Graph getGraph () const;
private:
	void findPolygon (const IntPoint& point, std::list<Polygon>& polygons);
	void clearBuffer ();

	void drawPolygon (const Polygon& polygon, Owner owner);
};

#endif // KDOTS_DOTTABLE_H
