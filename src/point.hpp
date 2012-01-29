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

#ifndef KDOTS_POINT_H
#define KDOTS_POINT_H

enum GameMode
{
	ExtraTurnGameMode,
	DefaultGameMode
};

enum Owner
{
	NoneOwner,
	FirstOwner,
	SecondOwner
};

template<class X>
class Point
{
	X xc, yc;
	bool isEmpty;
public:
	Point ()
	: isEmpty (true)
	{}
	
	Point (const X& xc, const X& yc)
	: xc (xc)
	, yc (yc)
	, isEmpty (false)
	{}

	
	X x () const
	{
		return xc;
	}
	
	X y () const
	{
		return yc;
	}
	
	void setX (const X& xc)
	{
		isEmpty = false;
		this->xc = xc;
	}
	
	void setY (const X& yc)
	{
		isEmpty = false;
		this->yc = yc;
	}
	
	bool operator== (const Point<X>& a) const
	{
		return xc == a.xc && yc == a.yc;
	}
	
	bool operator!= (const Point<X>& a) const
	{
		return xc != a.xc || yc != a.yc;
	}
	
	bool empty () const
	{
		return isEmpty;
	}
};

typedef Point<int> IntPoint;

#endif // KDOTS_POINT_H
