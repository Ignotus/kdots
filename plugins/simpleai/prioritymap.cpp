/*
 * KDots
 * Copyright (c) 2011, 2012, 2014, 2015 Minh Ngo <minh@fedoraproject.org>
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
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "prioritymap.hpp"

#include <boost/bimap.hpp>
#include <boost/range/combine.hpp>

#include <QFile>
#include <QTextStream>
#include <QStringList>

#include <config.hpp>

namespace KDots
{
namespace simpleai
{
  namespace
  {
    typedef boost::bimap<MapElement, QString> MapElementToStrType;
    const std::vector<MapElementToStrType::value_type> MAP_ELEMENT_TO_STR_VALUES = {
      {MapElement::EM, "EM"},
      {MapElement::NM, "NM"},
      {MapElement::FI, "FI"},
      {MapElement::SE, "SE"},
      {MapElement::PF, "PF"},
      {MapElement::PS, "PS"},
      {MapElement::CU, "CU"}
    };

    const MapElementToStrType MAP_ELEMENT_TO_STR(MAP_ELEMENT_TO_STR_VALUES.begin(),
                                                MAP_ELEMENT_TO_STR_VALUES.end());
  }

  MapData::MapData()
  {
  }

  MapData::MapData(const MapType& map, const Point& current, float priority)
    : m_map(map)
    , m_current(current)
    , m_priority(priority)
  {
  }

  QString MapData::toString() const
  {
    QString res;

    for (const auto& row : m_map)
    {
      res += "\n{ ";
      for (const auto& cell : row)
      {
        res += MAP_ELEMENT_TO_STR.left.at(cell) + " ";
      }
      res += "}";
    }

    res += "\n" + QString::number(m_current.m_x) + " " + QString::number(m_current.m_y) + "\n";

    return res;
  }
  
  namespace
  {
    const std::map<MapElement, MapElement> OPPOSITE_ELEMENT = {
      {MapElement::CU, MapElement::CU},
      {MapElement::EM, MapElement::EM},
      {MapElement::FI, MapElement::SE},
      {MapElement::SE, MapElement::FI},
      {MapElement::PF, MapElement::PS},
      {MapElement::PS, MapElement::PF},
      {MapElement::NM, MapElement::NM}
    };
  }
  
  MapData PriorityMap::opposite(const MapData& data)
  {
    const MapType& map = data.m_map;
    MapData newData;
    MapType& newMap = newData.m_map;

    newMap.resize(map.size());
    
    for (const boost::tuple<const MapLine&, MapLine&>& tup : boost::combine(map, newMap))
    {
      const MapLine& oldLine = tup.get<0>();
      MapLine& newLine = tup.get<1>();
      newLine.resize(oldLine.size());

      for (const boost::tuple<const MapElement&, MapElement&>& cell : boost::combine(oldLine, newLine))
        cell.get<1>() = OPPOSITE_ELEMENT.at(cell.get<0>());
    }
    
    newData.m_priority = data.m_priority;
    newData.m_current =  data.m_current;

    return newData;
  }

  MapData PriorityMap::inverse(const MapData& data)
  {
    const MapType& map = data.m_map;
    MapData newData;
    MapType& newMap = newData.m_map;

    newMap.resize(map.size());

    for (const boost::tuple<const MapLine&, MapLine&>& tup : boost::combine(map, newMap))
    {
      MapLine& newLine = tup.get<1>();
      const MapLine& oldLine = tup.get<0>();

      newLine.resize(oldLine.size());
      std::reverse_copy(oldLine.begin(), oldLine.end(), newLine.begin());
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
    for (MapLine& line : newMap)
      line.resize(map.size());

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
    : m_priorityMap(loadMap())
  {
  }

  PriorityMap& PriorityMap::instance()
  {
    static PriorityMap obj;
    return obj;
  }

  const QVector<MapData>& PriorityMap::priorityMap()
  {
    return m_priorityMap;
  }

  QVector<MapData> PriorityMap::loadMap() const
  {
    QVector<MapData> priorityMap;

    QFile file(PLUGINS_DIR "/rules");
    file.open(QIODevice::ReadOnly);

    QTextStream in(&file);

    while (!in.atEnd())
    {
      const float priority = in.readLine().toFloat();

      Point current;
      MapType map;
      for (int row = 0;; ++row)
      {
        const QString& line = in.readLine();
        if (line[0] == '/')
          break;

        const QStringList& cells = line.split(' ');
        map.push_back(MapLine(cells.size()));
        MapLine& lastRow = map.back();
        for (int column = 0; column < cells.size(); ++column)
        {
          const MapElement type = MAP_ELEMENT_TO_STR.right.at(cells[column]);

          lastRow[column] = type;

          if (type == MapElement::CU)
            current = {column, row};
        }
      }

      priorityMap.push_back(MapData(std::move(map), current, priority));
    }

    priorityMap.reserve(priorityMap.size() * 16);
    for (int i = 0, max = priorityMap.size(); i < max; ++i)
    {
      const MapData& data = priorityMap[i];
      
      priorityMap.push_back(opposite(data));
      
      const MapData& invertedData = inverse(data);
      priorityMap.push_back(invertedData);
      priorityMap.push_back(opposite(invertedData));

      MapData newData = data;
      for (int j = 0; j < 3; ++j)
      {
        newData = rotate(newData);
        
        priorityMap.push_back(newData);
        
        priorityMap.push_back(opposite(newData));
        const MapData& invertedMap = inverse(newData);
        priorityMap.push_back(invertedMap);
        priorityMap.push_back(opposite(invertedMap));
      }
    }
    
    return priorityMap;
  }
}
}