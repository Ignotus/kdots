/*
 * KDots
 * Copyright (c) 2011-2012 Minh Ngo <nlminhtl@gmail.com>
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef KDOTS_POINT_HPP
#define KDOTS_POINT_HPP
#include <QMetaType>
#include "constants.hpp"

namespace KDots
{
	class Point
	{
		int m_x, m_y;
	public:
		Point ()
			: m_x (-1)
			, m_y (-1)
		{}
		
		bool isInitialized () const
		{
			return m_x >= 0 && m_y >= 0;
		}
		
		Point (int x, int y)
			: m_x (x)
			, m_y (y)
		{
		}

		int x () const
		{
			return m_x;
		}

		int y () const
		{
			return m_y;
		}

		void setX (int x)
		{
			m_x = x;
		}

		void setY (int y)
		{
			m_y = y;
		}
		
		Point operator* (int val) const
		{
			return Point (m_x * val, m_y * val);
		}
		
		Point operator+ (int val) const
		{
			return Point (m_x + val, m_y + val);
		}
		
		Point operator- (const Point& point) const
		{
			return Point (m_x + point.m_x, m_y + point.m_y);
		}
		
		Point operator+ (const Point& point) const
		{
			return Point (m_x - point.m_x, m_y - point.m_y);
		}
		
		bool operator< (const Point& other) const
		{
			return m_x < other.m_x && m_y < other.m_y;
		}
		
		bool operator<= (const Point& other) const
		{
			return m_x <= other.m_x && m_y <= other.m_y;
		}
		
		bool operator> (const Point& other) const
		{
			return m_y > other.m_y && m_x > other.m_x;
		}
		
		bool operator>= (const Point& other) const
		{
			return m_y >= other.m_y && m_x >= other.m_x;
		}

		bool operator== (const Point& a) const
		{
			return m_x == a.m_x && m_y == a.m_y;
		}

		bool operator!= (const Point& a) const
		{
			return !(*this == a);
		}

		bool empty () const
		{
			return m_x == -1 || m_y == -1;
		}
		
		int sqrLength (const Point& second) const
		{
			const int dx = second.x () - m_x;
			const int dy = second.y () - m_y;
			return dx * dx + dy * dy;
		}
		
		static int sqrLength (const Point& first, const Point& second)
		{
			const int dx = second.x () - first.x ();
			const int dy = second.y () - first.y ();
			return dx * dx + dy * dy;
		}
		
		friend QDataStream& operator<< (QDataStream& out, const Point& obj);
		friend QDataStream& operator>> (QDataStream& in, Point& obj);
	};
	
	QDataStream& operator<< (QDataStream& out, const Point& obj);

	QDataStream& operator>> (QDataStream& in, Point& obj);
}

Q_DECLARE_METATYPE (KDots::Point);



#endif