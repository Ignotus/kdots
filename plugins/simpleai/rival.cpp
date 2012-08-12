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
#include "rival.hpp"
#include <list>
#include <include/point.hpp>
#include <include/dottable.hpp>

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
		
		const std::list<std::pair<MapType, float>> PRIORITY_MAP ({
			{
				{//1
					{NM, SE, NM},
					{FI, CU, FI},
					{NM, SE, NM}
				},
				1
			},
			{
				{//2
					{SE, PF, NM},
					{FI, CU, PF},
					{NM, SE, NM}
				},
				0.9
			},
			{
				{//3
					{NM, SE, PF},
					{FI, FI, CU},
					{NM, SE, PF}
				},
				0.9
			},
			{
				{//4
					{SE, NM, PF},
					{NM, CU, NM},
					{PF, NM, SE}
				},
				0.05
			},
			{
				{//5
					{NM, FI, NM, NM},
					{EM, EM, SE, NM},
					{NM, CU, EM, FI},
					{NM, NM, EM, NM}
				},
				0.8
			},
			{
				{//6
					{NM, FI, NM},
					{FI, SE, EM},
					{PF, CU, PF},
					{NM, SE, NM}
				},
				0.7
			},
			{
				{//7
					{NM, FI, NM},
					{FI, SE, EM},
					{NM, CU, NM}
				},
				0.2
			},
			{
				{//8
					{NM, EM, NM, NM},
					{FI, SE, CU, NM},
					{NM, FI, SE, EM},
					{NM, NM, FI, NM}
				},
				1
			},
			{
				{//9
					{NM, FI, NM},
					{PF, SE, CU},
					{NM, FI, NM}
				},
				0.2
			},
			{
				{//10
					{NM, SE, NM},
					{PF, CU, PF},
					{NM, SE, NM}
				},
				0.05
			},
			{
				{//11
					{PF, PF, CU},
					{EM, EM, EM},
					{PF, SE, PF},
					{NM, SE, NM}
				},
				0.01
			},
			{
				{//12
					{NM, PF, NM},
					{FI, EM, CU},
					{PF, SE, EM},
					{NM, SE, NM}
				},
				0.05
			},
			{
				{//13
					{NM, FI, NM},
					{FI, CU, FI}
				},
				-0.5
			},
			{
				{//14
					{NM, SE, NM},
					{NM, EM, NM},
					{PF, CU, PF},
					{NM, SE, NM}
				},
				0.05
			},
			{
				{//15
					{NM, SE, NM},
					{NM, EM, NM},
					{PF, CU, PF},
					{NM, EM, NM},
					{NM, SE, NM}
				},
				0.05
			},
			{
				{//16
					{NM, SE, NM},
					{NM, EM, NM},
					{PF, CU, PF},
					{NM, EM, NM},
					{NM, EM, NM},
					{NM, SE, NM}
				},
				0.05
			},
			{
				{//17
					{NM, SE, NM},
					{NM, EM, NM},
					{NM, EM, NM},
					{PF, CU, PF},
					{NM, EM, NM},
					{NM, EM, NM},
					{NM, SE, NM}
				},
				0.05
			},
			{
				{//18
					{NM, NM, SE},
					{PF, CU, PF},
					{NM, SE, NM}
				},
				0.05
			},
			{
				{//19
					{NM, SE, NM},
					{PF, CU, PF},
					{PS, EM, NM},
					{SE, NM, NM}					
				},
				0.05
			},
			{
				{//20
					{NM, SE, NM},
					{NM, EM, NM},
					{PF, CU, PF},
					{PS, EM, NM},
					{SE, NM, NM}
				},
				0.05
			},
			{
				{//21
					{NM, SE},
					{NM, EM},
					{PF, EM},
					{PF, CU},
					{SE, NM}
				},
				0.05
			},
			{
				{//22
					{NM, SE, NM},
					{PF, FI, CU},
					{NM, SE, NM}
				},
				0.1
			},
			{
				{//23
					{NM, SE, NM},
					{PF, FI, CU},
					{NM, EM, EM},
					{NM, SE, NM}
				},
				0.1
			},
			{
				{//24
					{NM, SE, NM},
					{NM, EM, EM},
					{PF, FI, CU},
					{NM, EM, EM},
					{NM, SE, NM}
				},
				0.1
			},
			{
				{//25
					{NM, SE},
					{PF, EM},
					{PF, CU},
					{SE, NM}
				},
				0.05
			},
			{
				{//26
					{SE, CU},
					{SE, SE}
				},
				-0.5
			},
			{
				{//27
					{NM, NM, EM, EM, NM},
					{NM, CU, EM, EM, SE},
					{EM, EM, FI, NM, NM},
					{NM, SE, NM, NM, NM}
				},
				0.8
			},
			{
				{//28
					{NM, NM, EM, EM, NM},
					{NM, CU, EM, EM, SE},
					{EM, EM, PF, FI, NM},
					{NM, SE, NM, NM, NM}
				},
				0.8
			},
			{
				{//29
					{FI, SE},
					{CU, FI},
					{SE, NM}
				},
				0.9
			},
			{
				{//30
					{NM, SE, NM},
					{NM, EM, EM},
					{NM, EM, EM},
					{PF, FI, CU},
					{NM, SE, NM}
				},
				0.1
			},
			{
				{//31
					{NM, SE, NM},
					{NM, EM, EM},
					{NM, EM, EM},
					{PF, FI, CU},
					{NM, EM, EM},
					{NM, SE, NM}
				},
				0.1
			},
			{
				{//32
					{NM, SE},
					{NM, EM},
					{FI, CU},
					{SE, NM}
				},
				0.1
			},
			{
				{//33
					{SE, FI, NM},
					{FI, CU, NM},
					{NM, NM, SE}
				},
				0.1
			}
		});
		
		namespace
		{
			MapType inverse (const MapType& map)
			{
				MapType newMap;
				newMap.resize (map.size ());
				
				MapType::const_iterator map_i = map.begin ();
				for (MapType::iterator new_i = newMap.begin (),
						new_e = newMap.end ();
						new_i != new_e; ++new_i, ++map_i)
				{
					new_i->resize (map_i->size ());
					std::reverse_copy (map_i->begin (), map_i->end (), new_i->begin ());
				}
				
				return newMap;
			}
		}
			
		Rival::Rival (QObject *parent)
			: IRival (parent)
			, m_table (NULL)
		{
		}
		
		bool Rival::isAllow () const
		{
		}
		
		namespace
		{
			int calcImportance (const DotTable *table, const Point& point)
			{
				return 1;
			}
		}
		
		void Rival::nextStep (const Point& point)
		{
		}
		
		void Rival::setDotTable (DotTable *table)
		{
			m_table = table;
		}
	}
}
