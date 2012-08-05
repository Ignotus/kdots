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
#include "point.hpp"

namespace KDots
{
	QDataStream& operator<< (QDataStream& out, const KDots::Point& obj)
	{
		out << (quint32) obj.m_x << (quint32) obj.m_y;
		return out;
	}

	QDataStream& operator>> (QDataStream& in, KDots::Point& obj)
	{
		in >> (quint32&) obj.m_x >> (quint32&) obj.m_y;
	}
}