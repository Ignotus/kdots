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

#include "gameconfig.hpp"

QDataStream& operator<< (QDataStream& out, const KDots::GameConfig& obj)
{
	out << (quint32) obj.m_firstOwner << (quint32) obj.m_height << (quint32) obj.m_width << (quint32) obj.m_mode;
	return out;
}

QDataStream& operator>> (QDataStream& in, KDots::GameConfig& obj)
{
	in >> (quint32&) obj.m_firstOwner >> (quint32&) obj.m_height >> (quint32&) obj.m_width >> (quint32&) obj.m_mode;
	return in;
}

void registerMeta ()
{
	qRegisterMetaType<KDots::GameConfig> ("GameConfig");
	qRegisterMetaTypeStreamOperators<KDots::GameConfig> ("GameConfig");
}
