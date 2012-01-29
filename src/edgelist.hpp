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

#ifndef KDOTS_EDGELIST_H
#define KDOTS_EDGELIST_H

#include "point.hpp"
#include <QDebug>

template <int A, class B>
class EdgeList
{
	int count;
	Point<B> pointList[A];
public:
	EdgeList ()
	: count (0)
	{}

	bool addEdge (const Point<B>& point)
	{
		if (count == A || hasPoint (point))
			return false;
		
		pointList[count++] = point;
		return true;
	}

	int size () const
	{
		return count;
	}

	bool hasPoint (const Point<B>& point)
	{
		for (int i = 0;  i < count; ++i)
		{
			if (pointList[i] == point)
				return true;
		}
		return false;
	}

	Point<B>& operator[] (const int index)
	{
		return index >= 0 && index < count
				? pointList[index]
				: Point<B> ();
	}
	
	const Point<B>& operator[] (const int index) const
	{
		Point<B> def;
		return index >= 0 && index < count
				? pointList[index]
				: def;
	}
	
	bool removeEdge (const Point<B>& toPoint)
	{
		int i = 0;
		for (; i < count; ++i)
		{
			if (pointList[i] == toPoint)
			{
				if (i != count)
					pointList[i] = toPoint;
				
				--count;
				return true;
			}
		}
		return false;
	}
};

#endif // KDOTS_EDGELIST_H
