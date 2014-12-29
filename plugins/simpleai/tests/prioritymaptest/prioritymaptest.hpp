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
#pragma once
#include <QtTest>
#include <QObject>
#include <rival.hpp>
#include <graph.hpp>
#include "prioritymap.hpp"
using namespace KDots::simpleai;

const MapData TEST_DATA {
  {
    {MapElement::NM, MapElement::CU, MapElement::SE},
    {MapElement::PF, MapElement::EM, MapElement::PF},
    {MapElement::PS, MapElement::NM, MapElement::PS},
    {MapElement::EM, MapElement::EM, MapElement::EM}
  },
  {1, 0},
  0.1
};

const MapData TEST_DATA_1 {
  {
    {MapElement::NM, MapElement::SE},
    {MapElement::PF, MapElement::EM},
    {MapElement::PS, MapElement::CU},
    {MapElement::EM, MapElement::EM}
  },
  {1, 2},
  0.1
};

class PriorityMapTest : public QObject
{
  Q_OBJECT
private slots:
  void maskCheckingTest()
  {
    KDots::Graph graph(3, 3);
    graph[KDots::Point(0, 1)].setOwner(KDots::Owner::FIRST);
    graph[KDots::Point(1, 0)].setOwner(KDots::Owner::FIRST);
    graph[KDots::Point(2, 1)].setOwner(KDots::Owner::FIRST);
    graph[KDots::Point(2, 2)].setOwner(KDots::Owner::FIRST);
    graph[KDots::Point(1, 1)].setOwner(KDots::Owner::SECOND);
    
    const MapData data {
      {
        {MapElement::NM, MapElement::FI, MapElement::NM},
        {MapElement::FI, MapElement::SE, MapElement::FI},
        {MapElement::EM, MapElement::CU, MapElement::PF}
      },
      {1, 2},
      0.9
    };
    
    const MapData& newData1 = PriorityMap::rotate(data);
    
    const MapData checkData1 {
      {
        {MapElement::EM, MapElement::FI, MapElement::NM},
        {MapElement::CU, MapElement::SE, MapElement::FI},
        {MapElement::PF, MapElement::FI, MapElement::NM}
      },
      {0, 1},
      0.9
    };
    QVERIFY(newData1 == checkData1);
    
    const MapData& newData2 = PriorityMap::rotate(newData1);
    
    const MapData checkData2 {
      {
        {MapElement::PF, MapElement::CU, MapElement::EM},
        {MapElement::FI, MapElement::SE, MapElement::FI},
        {MapElement::NM, MapElement::FI, MapElement::NM}
      },
      {1, 0},
      0.9
    };
    
    QVERIFY(newData2 == checkData2);
    
    const MapData& newData3 = PriorityMap::rotate(newData2);
    
    const MapData checkData3 {
      {
        {MapElement::NM, MapElement::FI, MapElement::PF},
        {MapElement::FI, MapElement::SE, MapElement::CU},
        {MapElement::NM, MapElement::FI, MapElement::EM}
      },
      {2, 1},
      0.9
    };
    
    QVERIFY(newData3 == checkData3);
    
    QVERIFY(KDots::simpleai::Rival::hasMask(graph, KDots::Point(1, 2), data, KDots::Owner::SECOND));
  }
  
  void sizeTest()
  {
    const std::list<MapData>& dataList = PriorityMap::instance().priorityMap();
    QVERIFY(dataList.size() % 8 == 0);
  }
  
  void currenPointTest()
  {
    const std::list<MapData>& dataList = PriorityMap::instance().priorityMap();
    int k = 0;
    for(const MapData& data : dataList)
    {
      ++k;
      QVERIFY(data.m_map[data.m_current.m_y][ data.m_current.m_x] == MapElement::CU);
      
    }
  }
  
  void inverseTest()
  {
    const MapData& inverted = PriorityMap::inverse(TEST_DATA);
    const MapData answer {
      {
        {MapElement::SE, MapElement::CU, MapElement::NM},
        {MapElement::PF,MapElement::EM, MapElement::PF},
        {MapElement::PS, MapElement::NM, MapElement::PS},
        {MapElement::EM, MapElement::EM,MapElement::EM}
      },
      {1, 0},
      0.1
    };
    
    QVERIFY(answer == inverted);
    
    const MapData& inverted_1 = PriorityMap::inverse(TEST_DATA_1);
    const MapData answer_1 {
      {
        {MapElement::SE, MapElement::NM},
        {MapElement::EM, MapElement::PF},
        {MapElement::CU, MapElement::PS},
        {MapElement::EM, MapElement::EM}
      },
      {0, 2},
      0.1
    };
    
    QVERIFY(answer_1 == inverted_1);
  }
  
  void rotateTest()
  {
    const MapData& rotated = PriorityMap::rotate(TEST_DATA);
    const MapData answer {
      {
        {MapElement::EM, MapElement::PS, MapElement::PF, MapElement::NM},
        {MapElement::EM, MapElement::NM, MapElement::EM, MapElement::CU},
        {MapElement::EM, MapElement::PS, MapElement::PF, MapElement::SE}
      },
      {3, 1},
      0.1
    };
    
    qDebug() << answer.toString();
    
    QVERIFY(rotated == answer);
    
    const MapData& rotated_1 = PriorityMap::rotate(TEST_DATA_1);
    const MapData answer_1 {
      {
        {MapElement::EM, MapElement::PS, MapElement::PF, MapElement::NM},
        {MapElement::EM, MapElement::CU, MapElement::EM, MapElement::SE}
      },
      {1, 1},
      0.1
    };
    
    QVERIFY(rotated_1 == answer_1);
  }
};

