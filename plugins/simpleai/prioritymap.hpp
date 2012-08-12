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
#ifndef KDOTS_PLUGINS_SIMPLEAI_PRIORITY_HPP
#define KDOTS_PLUGINS_SIMPLEAI_PRIORITY_HPP
#include <list>
#include <vector>
#include <include/point.hpp>

struct stat;
namespace KDots
{
	namespace simpleai
	{
		enum MapElement
		{
			EM, //Empty
			NM, // Does not matter
			FI, //First
			SE, //Second
			PF, // Possibly first
			PS, // Possibly second
			CU // Current
		};
		
		typedef std::vector<MapElement> MapLine;
		typedef std::vector<MapLine> MapType;
		
		struct MapData
		{
			MapType m_map;
			Point m_current;
			float m_priority;

			inline MapData ()
			{
			}
			
			inline MapData (const MapType& map, const Point& current, float priority)
				: m_map (map)
				, m_current (current)
				, m_priority (priority)
			{
			}
			
			bool operator== (const MapData& other) const;
			bool operator!= (const MapData& other) const;
		};
		
		class PriorityMap
		{
			std::list<MapData> m_priorityMap;
			
			PriorityMap ();
			PriorityMap (const PriorityMap&);
			PriorityMap& operator= (const PriorityMap&);
		public:
			static PriorityMap& instance ();
			
			static MapData inverse (const MapData& data);
			static MapData rotate (const MapData& data);
			
			const std::list<MapData>& priorityMap ();
		};
	}
}

#endif
