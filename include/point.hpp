/*
 * kdots
 * Copyright (C) 2011-2012 Minh Ngo <nlminhtl@gmail.com>
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
#ifndef KDOTS_POINT_HPP
#define KDOTS_POINT_HPP
#include <QMetaType>
#include "constants.hpp"

namespace KDots
{
	class KDOTS_EXPORT Point
	{
		int m_x, m_y;
	public:
		inline Point ()
			: m_x (-1)
			, m_y (-1)
		{}
		
		inline Point (int x, int y)
			: m_x (x)
			, m_y (y)
		{}

		inline int x () const
		{
			return m_x;
		}

		inline int y () const
		{
			return m_y;
		}

		inline void setX (int x)
		{
			m_x = x;
		}

		inline void setY (int y)
		{
			m_y = y;
		}

		inline bool operator== (const Point& a) const
		{
			return m_x == a.m_x && m_y == a.m_y;
		}

		inline bool operator!= (const Point& a) const
		{
			return ! (*this == a);
		}

		inline bool empty () const
		{
			return m_x == -1 || m_y == -1;
		}
		
		friend QDataStream& operator<< (QDataStream& out, const KDots::Point& obj);
		friend QDataStream& operator>> (QDataStream& in, KDots::Point& obj);
	};
	
	QDataStream& operator<< (QDataStream& out, const KDots::Point& obj);

	QDataStream& operator>> (QDataStream& in, KDots::Point& obj);
}

Q_DECLARE_METATYPE (KDots::Point);



#endif