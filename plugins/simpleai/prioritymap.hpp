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
#ifndef KDOTS_PLUGINS_SIMPLEAI_PRIORITY_HPP
#define KDOTS_PLUGINS_SIMPLEAI_PRIORITY_HPP
#include <list>
#include <vector>
#include <KDebug>
#include <constants.hpp>
#include <point.hpp>

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
    
    struct KDOTS_EXPORT MapData
    {
      MapType m_map;
      Point m_current;
      float m_priority;

      MapData()
      {
      }
      
      MapData(const MapType& map, const Point& current, float priority)
        : m_map(map)
        , m_current(current)
        , m_priority(priority)
      {
      }
      
      QString toString() const
      {
        QString res;
        for(std::size_t j = 0; j < m_map.size(); ++j)
        {
          res += "\n{ ";
          for(std::size_t i = 0; i < m_map.front().size(); ++i)
          {
            switch(m_map[j][i])
            {
              case EM: //Empty
                res += "EM ";
                break;
              case NM: // Does not matter
                res += "NM ";
                break;
              case FI: //First
                res += "FI ";
                break;
              case SE: //Second
                res += "SE ";
                break;
              case PF: // Possibly first
                res += "PF ";
                break;
              case PS: // Possibly second
                res += "PS ";
                break;
              case CU: // Current
                res += "CU ";
                break;
              default:
                kDebug() << "WTF?";
            }
          }
          res += "}";
          
        }
        
        res += "\n";
        
        return res;
      }
      
      bool operator==(const MapData& other) const;
      bool operator!=(const MapData& other) const;
    };
    
    class KDOTS_EXPORT PriorityMap
    {
      std::list<MapData> m_priorityMap;
      
      PriorityMap();
      PriorityMap(const PriorityMap&);
      PriorityMap& operator=(const PriorityMap&);
    public:
      static PriorityMap& instance();
      
      static MapData inverse(const MapData& data);
      static MapData rotate(const MapData& data);
      
      const std::list<MapData>& priorityMap();
    };
  }
}

#endif
