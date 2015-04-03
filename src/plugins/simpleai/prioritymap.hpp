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
#pragma once

#include <boost/noncopyable.hpp>

#include <QVector>

#include <kdotslib/constants.hpp>
#include <kdotslib/point.hpp>

struct stat;
namespace KDots
{
namespace simpleai
{
  enum class MapElement
  {
    EM, //Empty
    NM, // Does not matter
    FI, //First
    SE, //Second
    PF, // Possibly first
    PS, // Possibly second
    CU // Current
  };
  
  typedef QVector<MapElement> MapLine;
  typedef QVector<MapLine> MapType;
  
  class MapData
  {
  public:
    MapData();
    MapData(const MapType& map, const Point& current, float priority);

    QString toString() const;

  public:
    MapType m_map;
    Point m_current;
    float m_priority;
  };
  
  class PriorityMap : boost::noncopyable
  {
  public:
    static PriorityMap& instance();

    const QVector<MapData>& priorityMap();

  private:
    PriorityMap();

    static MapData inverse(const MapData& data);
    static MapData rotate(const MapData& data);
    static MapData opposite(const MapData& data);
    QVector<MapData> loadMap() const;

  private:
    QVector<MapData> m_priorityMap;
  };
}
}