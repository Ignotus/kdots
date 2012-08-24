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
#include "prioritymap.hpp"

namespace KDots
{
	namespace simpleai
	{
		const std::list<MapData> PRIORITY_MAP {
			{
				{//1
					{NM, SE, NM},
					{FI, CU, FI},
					{NM, SE, NM}
				},
				{1, 1},
				1
			},
			{
				{//2
					{SE, PF, NM},
					{FI, CU, PF},
					{NM, SE, NM}
				},
				{1, 1},
				0.9
			},
			{
				{//3
					{NM, SE, PF},
					{FI, FI, CU},
					{NM, SE, PF}
				},
				{2, 1},
				0.9
			},
			{
				{//4
					{SE, NM, PF},
					{NM, CU, NM},
					{PF, NM, SE}
				},
				{1, 1},
				0.05
			},
			{
				{//5
					{NM, FI, NM, NM},
					{EM, EM, SE, NM},
					{NM, CU, EM, FI},
					{NM, NM, EM, NM}
				},
				{1, 2},
				0.8
			},
			{
				{//6
					{NM, FI, NM},
					{FI, SE, EM},
					{PF, CU, PF},
					{NM, SE, NM}
				},
				{1, 2},
				0.7
			},
			{
				{//7
					{NM, FI, NM},
					{FI, SE, EM},
					{NM, CU, NM}
				},
				{1, 2},
				0.2
			},
			{
				{//8
					{NM, EM, NM, NM},
					{FI, SE, CU, NM},
					{NM, FI, SE, EM},
					{NM, NM, FI, NM}
				},
				{2, 1},
				1
			},
			{
				{//9
					{NM, FI, NM},
					{PF, SE, CU},
					{NM, FI, NM}
				},
				{2, 1},
				0.2
			},
			{
				{//10
					{NM, SE, NM},
					{PF, CU, PF},
					{NM, SE, NM}
				},
				{1, 1},
				0.05
			},
			{
				{//11
					{PF, PF, CU},
					{EM, EM, EM},
					{PF, SE, PF},
					{NM, SE, NM}
				},
				{2, 0},
				0.01
			},
			{
				{//12
					{NM, PF, NM},
					{FI, EM, CU},
					{PF, SE, EM},
					{NM, SE, NM}
				},
				{2, 1},
				0.05
			},
			{
				{//13
					{NM, FI, NM},
					{FI, CU, FI}
				},
				{1, 1},
				-0.5
			},
			{
				{//14
					{NM, SE, NM},
					{NM, EM, NM},
					{PF, CU, PF},
					{NM, SE, NM}
				},
				{1, 2},
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
				{1, 2},
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
				{1, 2},
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
				{1, 3},
				0.05
			},
			{
				{//18
					{NM, NM, SE},
					{PF, CU, PF},
					{NM, SE, NM}
				},
				{1, 1},
				0.05
			},
			{
				{//19
					{NM, SE, NM},
					{PF, CU, PF},
					{PS, EM, NM},
					{SE, NM, NM}					
				},
				{1, 1},
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
				{1, 2},
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
				{1, 3},
				0.05
			},
			{
				{//22
					{NM, SE, NM},
					{PF, FI, CU},
					{NM, SE, NM}
				},
				{2, 1},
				0.1
			},
			{
				{//23
					{NM, SE, NM},
					{PF, FI, CU},
					{NM, EM, EM},
					{NM, SE, NM}
				},
				{2, 1},
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
				{2, 2},
				0.1
			},
			{
				{//25
					{NM, SE},
					{PF, EM},
					{PF, CU},
					{SE, NM}
				},
				{1, 2},
				0.05
			},
			{
				{//26
					{SE, CU},
					{SE, SE}
				},
				{1, 0},
				-0.5
			},
			{
				{//27
					{NM, NM, EM, EM, NM},
					{NM, CU, EM, EM, SE},
					{EM, EM, FI, NM, NM},
					{NM, SE, NM, NM, NM}
				},
				{1, 1},
				0.8
			},
			{
				{//28
					{NM, NM, EM, EM, NM},
					{NM, CU, EM, EM, SE},
					{EM, EM, PF, FI, NM},
					{NM, SE, NM, NM, NM}
				},
				{1, 1},
				0.8
			},
			{
				{//29
					{FI, SE},
					{CU, FI},
					{SE, NM}
				},
				{0, 1},
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
				{2, 3},
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
				{2, 3},
				0.1
			},
			{
				{//32
					{NM, SE},
					{NM, EM},
					{FI, CU},
					{SE, NM}
				},
				{1, 2},
				0.1
			},
			{
				{//33
					{SE, FI, NM},
					{FI, CU, NM},
					{NM, NM, SE}
				},
				{1, 1},
				0.1
			}
		};
		
		bool MapData::operator== (const MapData& other) const
		{
			if (m_current != other.m_current)
				return false;
			
			if (m_priority != other.m_priority)
				return false;
			
			if (other.m_map.size () != m_map.size ()
					|| !other.m_map.size ()
					|| !m_map.size ())
				return false;
			
			for (MapType::const_iterator other_itr = other.m_map.begin (),
					itr = m_map.begin (), other_e = other.m_map.end ();
					other_itr != other_e ; ++other_itr, ++itr)
			{
				if (!std::equal (other_itr->begin (), other_itr->end (), itr->begin ()))
					return false;
			}
			
			return true;
		}
		
		bool MapData::operator!= (const MapData& other) const
		{
			return !(other == *this);
		}
		
		///
		
		MapData PriorityMap::inverse (const MapData& data)
		{
			const MapType& map = data.m_map;
			MapData newData;
			MapType& newMap = newData.m_map;
			
			newMap.resize (map.size ());
			
			MapType::const_iterator map_i = map.begin ();
			for (MapType::iterator new_i = newMap.begin (),
					new_e = newMap.end ();
					new_i != new_e; ++new_i, ++map_i)
			{
				new_i->resize (map_i->size ());
				std::reverse_copy (map_i->begin (), map_i->end (), new_i->begin ());
			}
			
			newData.m_priority = data.m_priority;
			newData.m_current = {static_cast<int> (map.front ().size () - 1 - data.m_current.x ()),
					data.m_current.y ()};
			
			return newData;
		}
		
		MapData PriorityMap::rotate (const MapData& data) 
		{
			const MapType& map = data.m_map;
			MapData newData;
			MapType& newMap = newData.m_map;
			
			newMap.resize (map.front ().size ());
			std::for_each (newMap.begin (), newMap.end (), [&map] (MapLine& line) {
				line.resize (map.size ());
			});
			
			for (int i = 0, max_i = newMap.size (), j, max_j = map.size (); i != max_i; ++i) //y
			{
				for (j = 0; j != max_j; ++j) //x
					newMap[i][j] = map[max_j - 1 - j][i]; 
			}
			
			newData.m_priority = data.m_priority;
			newData.m_current = {static_cast<int> (map.size () - 1 - data.m_current.y ()),
					data.m_current.x ()};
			
			return newData;
		}
		
		PriorityMap::PriorityMap ()
			: m_priorityMap (PRIORITY_MAP)
		{
			for (const MapData& data : PRIORITY_MAP)
			{
				const MapData& invertedData = inverse (data);
				m_priorityMap.push_back (invertedData);
				
				MapData newData = data;
				for (int i = 0; i < 3; ++i)
				{
					newData = rotate (newData);
					m_priorityMap.push_back (newData);
					const MapData& invertedMap = inverse (newData);
					m_priorityMap.push_back (invertedMap);
				}
			}
		}
		
		PriorityMap& PriorityMap::instance ()
		{
			static PriorityMap obj;
			return obj;
		}
		
		const std::list<MapData>& PriorityMap::priorityMap ()
		{
			return m_priorityMap;
		}
	}
}