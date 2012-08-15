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
#ifndef KDOTS_EDGELIST_HPP
#define KDOTS_EDGELIST_HPP
#include "point.hpp"

namespace KDots
{
	template<int SIZE>
	class KDOTS_EXPORT EdgeList
	{
		int m_count;
		Point m_pointList[SIZE];
	public:
		inline EdgeList()
			: m_count (0)
		{}

		inline bool addEdge (const Point& point)
		{
			if (m_count == SIZE || hasPoint (point))
				return false;

			m_pointList[m_count++] = point;
			return true;
		}

		inline int size () const
		{
			return m_count;
		}

		inline bool hasPoint (const Point& point)
		{
			for (int i = 0;  i < m_count; ++i)
			{
				if (m_pointList[i] == point)
					return true;
			}

			return false;
		}

		inline Point& operator[] (int index)
		{
			return const_cast<EdgeList<SIZE>&> (static_cast<const EdgeList<SIZE>&> (*this) [index]);
		}

		inline const Point& operator[] (int index) const
		{
			Point def;
			return index >= 0 && index < m_count ? m_pointList[index] : def;
		}

		inline bool removeEdge (const Point& toPoint)
		{
			for (int i = 0; i < m_count; ++i)
			{
				if (m_pointList[i] == toPoint)
				{
					m_count--;

					if (i != m_count)
						m_pointList[i] = m_pointList[m_count];

					return true;
				}
			}

			return false;
		}
	};
}

#endif
