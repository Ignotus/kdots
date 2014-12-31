/*
 * KDots
 * Copyright(c) 2011-2012 Minh Ngo <nlminhtl@gmail.com>
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
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
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
          {MapElement::NM, MapElement::SE, MapElement::NM},
          {MapElement::FI, MapElement::CU, MapElement::FI},
          {MapElement::NM, MapElement::SE, MapElement::NM}
        },
        {1, 1},
        0.95
      },
      {
        {//2
          {MapElement::SE, MapElement::PF, MapElement::NM},
          {MapElement::FI, MapElement::CU, MapElement::PF},
          {MapElement::NM, MapElement::SE, MapElement::NM}
        },
        {1, 1},
        0.9
      },
      {
        {//3
          {MapElement::NM, MapElement::SE, MapElement::PF},
          {MapElement::FI, MapElement::FI, MapElement::CU},
          {MapElement::NM, MapElement::SE, MapElement::PF}
        },
        {2, 1},
        0.9
      },
      {
        {//4
          {MapElement::SE, MapElement::NM, MapElement::PF},
          {MapElement::NM, MapElement::CU, MapElement::NM},
          {MapElement::PF, MapElement::NM, MapElement::SE}
        },
        {1, 1},
        0.05
      },
      {
        {//5
          {MapElement::NM, MapElement::FI, MapElement::NM, MapElement::NM},
          {MapElement::EM, MapElement::EM, MapElement::SE, MapElement::NM},
          {MapElement::NM, MapElement::CU, MapElement::EM, MapElement::FI},
          {MapElement::NM, MapElement::NM, MapElement::EM, MapElement::NM}
        },
        {1, 2},
        0.8
      },
      {
        {//6
          {MapElement::NM, MapElement::FI, MapElement::NM},
          {MapElement::FI, MapElement::SE, MapElement::EM},
          {MapElement::PF, MapElement::CU, MapElement::PF},
          {MapElement::NM, MapElement::SE, MapElement::NM}
        },
        {1, 2},
        0.7
      },
      {
        {//7
          {MapElement::NM, MapElement::FI, MapElement::NM},
          {MapElement::FI, MapElement::SE, MapElement::EM},
          {MapElement::NM, MapElement::CU, MapElement::NM}
        },
        {1, 2},
        0.2
      },
      {
        {//8
          {MapElement::NM, MapElement::EM, MapElement::NM, MapElement::NM},
          {MapElement::FI, MapElement::SE, MapElement::CU, MapElement::NM},
          {MapElement::NM, MapElement::FI, MapElement::SE, MapElement::EM},
          {MapElement::NM, MapElement::NM, MapElement::FI, MapElement::NM}
        },
        {2, 1},
        0.95
      },
      {
        {//9
          {MapElement::NM, MapElement::FI, MapElement::NM},
          {MapElement::PF, MapElement::SE, MapElement::CU},
          {MapElement::NM, MapElement::FI, MapElement::NM}
        },
        {2, 1},
        0.2
      },
      {
        {//10
          {MapElement::NM, MapElement::SE, MapElement::NM},
          {MapElement::PF, MapElement::CU, MapElement::PF},
          {MapElement::NM, MapElement::SE, MapElement::NM}
        },
        {1, 1},
        0.05
      },
      {
        {//11
          {MapElement::PF, MapElement::PF, MapElement::CU},
          {MapElement::EM, MapElement::EM, MapElement::EM},
          {MapElement::PF, MapElement::SE, MapElement::PF},
          {MapElement::NM, MapElement::SE, MapElement::NM}
        },
        {2, 0},
        0.01
      },
      {
        {//12
          {MapElement::NM, MapElement::PF, MapElement::NM},
          {MapElement::FI, MapElement::EM, MapElement::CU},
          {MapElement::PF, MapElement::SE, MapElement::EM},
          {MapElement::NM, MapElement::SE, MapElement::NM}
        },
        {2, 1},
        0.05
      },
      {
        {//13
          {MapElement::NM, MapElement::FI, MapElement::NM},
          {MapElement::FI, MapElement::CU, MapElement::FI}
        },
        {1, 1},
        -0.5
      },
      {
        {//14
          {MapElement::NM, MapElement::SE, MapElement::NM},
          {MapElement::NM, MapElement::EM, MapElement::NM},
          {MapElement::PF, MapElement::CU, MapElement::PF},
          {MapElement::NM, MapElement::SE, MapElement::NM}
        },
        {1, 2},
        0.05
      },
      {
        {//15
          {MapElement::NM, MapElement::SE, MapElement::NM},
          {MapElement::NM, MapElement::EM, MapElement::NM},
          {MapElement::PF, MapElement::CU, MapElement::PF},
          {MapElement::NM, MapElement::EM, MapElement::NM},
          {MapElement::NM, MapElement::SE, MapElement::NM}
        },
        {1, 2},
        0.05
      },
      {
        {//16
          {MapElement::NM, MapElement::SE, MapElement::NM},
          {MapElement::NM, MapElement::EM, MapElement::NM},
          {MapElement::PF, MapElement::CU, MapElement::PF},
          {MapElement::NM, MapElement::EM, MapElement::NM},
          {MapElement::NM, MapElement::EM, MapElement::NM},
          {MapElement::NM, MapElement::SE, MapElement::NM}
        },
        {1, 2},
        0.05
      },
      {
        {//17
          {MapElement::NM, MapElement::SE, MapElement::NM},
          {MapElement::NM, MapElement::EM, MapElement::NM},
          {MapElement::NM, MapElement::EM, MapElement::NM},
          {MapElement::PF, MapElement::CU, MapElement::PF},
          {MapElement::NM, MapElement::EM, MapElement::NM},
          {MapElement::NM, MapElement::EM, MapElement::NM},
          {MapElement::NM, MapElement::SE, MapElement::NM}
        },
        {1, 3},
        0.05
      },
      {
        {//18
          {MapElement::NM, MapElement::NM, MapElement::SE},
          {MapElement::PF, MapElement::CU, MapElement::PF},
          {MapElement::NM, MapElement::SE, MapElement::NM}
        },
        {1, 1},
        0.05
      },
      {
        {//19
          {MapElement::NM, MapElement::SE, MapElement::NM},
          {MapElement::PF, MapElement::CU, MapElement::PF},
          {MapElement::PS, MapElement::EM, MapElement::NM},
          {MapElement::SE, MapElement::NM, MapElement::NM}          
        },
        {1, 1},
        0.05
      },
      {
        {//20
          {MapElement::NM, MapElement::SE, MapElement::NM},
          {MapElement::NM, MapElement::EM, MapElement::NM},
          {MapElement::PF, MapElement::CU, MapElement::PF},
          {MapElement::PS, MapElement::EM, MapElement::NM},
          {MapElement::SE, MapElement::NM, MapElement::NM}
        },
        {1, 2},
        0.05
      },
      {
        {//21
          {MapElement::NM, MapElement::SE},
          {MapElement::NM, MapElement::EM},
          {MapElement::PF, MapElement::EM},
          {MapElement::PF, MapElement::CU},
          {MapElement::SE, MapElement::NM}
        },
        {1, 3},
        0.05
      },
      {
        {//22
          {MapElement::NM, MapElement::SE, MapElement::NM},
          {MapElement::PF, MapElement::FI, MapElement::CU},
          {MapElement::NM, MapElement::SE, MapElement::NM}
        },
        {2, 1},
        0.1
      },
      {
        {//23
          {MapElement::NM, MapElement::SE, MapElement::NM},
          {MapElement::PF, MapElement::FI, MapElement::CU},
          {MapElement::NM, MapElement::EM, MapElement::EM},
          {MapElement::NM, MapElement::SE, MapElement::NM}
        },
        {2, 1},
        0.1
      },
      {
        {//24
          {MapElement::NM, MapElement::SE, MapElement::NM},
          {MapElement::NM, MapElement::EM, MapElement::EM},
          {MapElement::PF, MapElement::FI, MapElement::CU},
          {MapElement::NM, MapElement::EM, MapElement::EM},
          {MapElement::NM, MapElement::SE, MapElement::NM}
        },
        {2, 2},
        0.1
      },
      {
        {//25
          {MapElement::NM, MapElement::SE},
          {MapElement::PF, MapElement::EM},
          {MapElement::PF, MapElement::CU},
          {MapElement::SE, MapElement::NM}
        },
        {1, 2},
        0.05
      },
      {
        {//26
          {MapElement::SE, MapElement::CU},
          {MapElement::SE, MapElement::SE}
        },
        {1, 0},
        -0.5
      },
      {
        {//27
          {MapElement::NM, MapElement::NM, MapElement::EM, MapElement::EM, MapElement::NM},
          {MapElement::NM, MapElement::CU, MapElement::EM, MapElement::EM, MapElement::SE},
          {MapElement::EM, MapElement::EM, MapElement::FI, MapElement::NM, MapElement::NM},
          {MapElement::NM, MapElement::SE, MapElement::NM, MapElement::NM, MapElement::NM}
        },
        {1, 1},
        0.8
      },
      {
        {//28
          {MapElement::NM, MapElement::NM, MapElement::EM, MapElement::EM, MapElement::NM},
          {MapElement::NM, MapElement::CU, MapElement::EM, MapElement::EM, MapElement::SE},
          {MapElement::EM, MapElement::EM, MapElement::PF, MapElement::FI, MapElement::NM},
          {MapElement::NM, MapElement::SE, MapElement::NM, MapElement::NM, MapElement::NM}
        },
        {1, 1},
        0.8
      },
      {
        {//29
          {MapElement::FI, MapElement::SE},
          {MapElement::CU, MapElement::FI},
          {MapElement::SE, MapElement::NM}
        },
        {0, 1},
        0.9
      },
      {
        {//30
          {MapElement::NM, MapElement::SE, MapElement::NM},
          {MapElement::NM, MapElement::EM, MapElement::EM},
          {MapElement::NM, MapElement::EM, MapElement::EM},
          {MapElement::PF, MapElement::FI, MapElement::CU},
          {MapElement::NM, MapElement::SE, MapElement::NM}
        },
        {2, 3},
        0.1
      },
      {
        {//31
          {MapElement::NM, MapElement::SE, MapElement::NM},
          {MapElement::NM, MapElement::EM, MapElement::EM},
          {MapElement::NM, MapElement::EM, MapElement::EM},
          {MapElement::PF, MapElement::FI, MapElement::CU},
          {MapElement::NM, MapElement::EM, MapElement::EM},
          {MapElement::NM, MapElement::SE, MapElement::NM}
        },
        {2, 3},
        0.1
      },
      {
        {//32
          {MapElement::NM, MapElement::SE},
          {MapElement::NM, MapElement::EM},
          {MapElement::FI, MapElement::CU},
          {MapElement::SE, MapElement::NM}
        },
        {1, 2},
        0.1
      },
      {
        {//33
          {MapElement::SE, MapElement::FI, MapElement::NM},
          {MapElement::FI, MapElement::CU, MapElement::NM},
          {MapElement::NM, MapElement::NM, MapElement::SE}
        },
        {1, 1},
        0.1
      },
      {
        {
          {MapElement::PS, MapElement::PS, MapElement::NM},
          {MapElement::PS, MapElement::CU, MapElement::FI},
          {MapElement::FI, MapElement::SE, MapElement::PS}
        },
        {1, 1},
        0.4
      },
      {
        {
          {MapElement::NM, MapElement::EM, MapElement::NM, MapElement::NM},
          {MapElement::SE, MapElement::FI, MapElement::CU, MapElement::NM},
          {MapElement::NM, MapElement::SE, MapElement::FI, MapElement::EM},
          {MapElement::NM, MapElement::PS, MapElement::SE, MapElement::PS}
        },
        {2, 1},
        1.0
      }
    };
    
    bool MapData::operator==(const MapData& other) const
    {
      if (m_current != other.m_current)
        return false;
      
      if (m_priority != other.m_priority)
        return false;
      
      if (other.m_map.size() != m_map.size() || !other.m_map.size() || !m_map.size())
        return false;
      
      for (MapType::const_iterator other_itr = other.m_map.begin(),
           itr = m_map.begin(), other_e = other.m_map.end();
           other_itr != other_e ; ++other_itr, ++itr)
      {
        if (!std::equal(other_itr->begin(), other_itr->end(), itr->begin()))
          return false;
      }
      
      return true;
    }
    
    bool MapData::operator!=(const MapData& other) const
    {
      return !(other == *this);
    }
    
    ///
    
    MapData PriorityMap::inverse(const MapData& data)
    {
      const MapType& map = data.m_map;
      MapData newData;
      MapType& newMap = newData.m_map;
      
      newMap.resize(map.size());
      
      MapType::const_iterator map_i = map.begin();
      for (MapType::iterator new_i = newMap.begin(),
          new_e = newMap.end();
          new_i != new_e; ++new_i, ++map_i)
      {
        new_i->resize(map_i->size());
        std::reverse_copy(map_i->begin(), map_i->end(), new_i->begin());
      }
      
      newData.m_priority = data.m_priority;
      newData.m_current = {static_cast<int>(map.front().size() - 1 - data.m_current.m_x),
          data.m_current.m_y};
      
      return newData;
    }
    
    MapData PriorityMap::rotate(const MapData& data) 
    {
      const MapType& map = data.m_map;
      MapData newData;
      MapType& newMap = newData.m_map;
      
      newMap.resize(map.front().size());
      std::for_each(newMap.begin(), newMap.end(), [&map](MapLine& line) {
        line.resize(map.size());
      });
      
      for (int i = 0, max_i = newMap.size(), j, max_j = map.size(); i != max_i; ++i) //y
      {
        for (j = 0; j != max_j; ++j) //x
          newMap[i][j] = map[max_j - 1 - j][i]; 
      }
      
      newData.m_priority = data.m_priority;
      newData.m_current = {static_cast<int>(map.size() - 1 - data.m_current.m_y),
          data.m_current.m_x};
      
      return newData;
    }
    
    PriorityMap::PriorityMap()
      : m_priorityMap(PRIORITY_MAP)
    {
      for (const MapData& data : PRIORITY_MAP)
      {
        const MapData& invertedData = inverse(data);
        m_priorityMap.push_back(invertedData);
        
        MapData newData = data;
        for (int i = 0; i < 3; ++i)
        {
          newData = rotate(newData);
          m_priorityMap.push_back(newData);
          const MapData& invertedMap = inverse(newData);
          m_priorityMap.push_back(invertedMap);
        }
      }
    }
    
    PriorityMap& PriorityMap::instance()
    {
      static PriorityMap obj;
      return obj;
    }
    
    const std::list<MapData>& PriorityMap::priorityMap()
    {
      return m_priorityMap;
    }
  }
}