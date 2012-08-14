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
#ifndef KDOTS_POLYGON_HPP
#define KDOTS_POLYGON_HPP
#include <list>
#include <vector>
#include <memory>
#include "point.hpp"
#include "constants.hpp"

namespace KDots
{
	class Polygon : public std::list<Point>
	{
		bool m_filled;
		Owner m_owner;
	public:
		Polygon ()
			: std::list<Point> ()
			, m_filled (false)
		{
		}

		Polygon (const std::list<Point>& a)
			: std::list<Point> (a)
			, m_filled (false)
		{
		}

		bool isFilled () const
		{
			return m_filled;
		}

		void setFilled (bool filled)
		{
			m_filled = filled;
		}
		
		Owner owner () const
		{
			return m_owner;
		}
		
		void setOwner (Owner own)
		{
			m_owner = own;
		}
	};
	
	typedef std::shared_ptr<Polygon> Polygon_ptr;
	typedef std::vector<Polygon_ptr> PolyList;
}

#endif