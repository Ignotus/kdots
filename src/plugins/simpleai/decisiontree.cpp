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
#include "decisiontree.hpp"

#include <stack>

#include <QRect>
#include <QDebug>

#include <kdotslib/graph.hpp>
#include <kdotslib/stepqueue.hpp>
#include <kdotslib/polygonfinder.hpp>

namespace KDots
{
namespace simpleai
{
  NodeInfo::NodeInfo()
    : m_parent(-1)
    , m_layer(0)
    , m_bestChildGrade(0)
    , m_capturedPointsCount(0)
    , m_point{-1, -1}
  {
  }
  
  DecisionTree::DecisionTree(const Graph& graph, const QRect& bbox,
                             int numPointsOnBoard, int depth, Owner ai)
    : m_graph(graph)
    , m_bbox(bbox)
    , m_numPointsOnBoard(numPointsOnBoard)
    , m_maxNumPoints(graph.width() * graph.height())
    , m_depth(depth)
    , m_ai(ai)
  {
    qDebug() << "Bounding box:" << bbox.width() << bbox.height();
  }
  
  void DecisionTree::calculateDecisions(std::vector<Point>& points, VectorF& grades)
  {
    m_nodes = {NodeInfo()};
    
    // For DFS
    std::stack<std::size_t> tasks;
    tasks.push(0);
    
    while (!tasks.empty())
    {
      const int parentNodeID = tasks.top();
      tasks.pop();

      TPreviousPoints previousPoints;

      findPreviousPoints(parentNodeID, previousPoints);

      std::vector<VectorB> allowedPoints(m_bbox.width(), VectorB(m_bbox.height(), false));
      buildAllowedPointsMap(previousPoints, allowedPoints);

      const std::size_t layerStart = m_nodes.size();
      
      const int parentLayer = m_nodes[parentNodeID].m_layer;
      const int currentLayer = parentLayer + 1;
      const bool isHumanLayer = parentLayer % 2 == 1;
      
      const Owner layerOwner = isHumanLayer ? StepQueue::other(m_ai) : m_ai;
      
      const std::vector<Point>& ownerPrevPoints = isHumanLayer
          ? previousPoints.m_human
          : previousPoints.m_ai;
      
      for (int x = 0; x < m_bbox.width(); ++x)
      {
        for (int y = 0; y < m_bbox.height(); ++y)
        {
          if (!allowedPoints[x][y])
            continue;

          m_nodes.emplace_back();

          NodeInfo& newNode = m_nodes[m_nodes.size() - 1];
          newNode.m_layer = currentLayer;
          newNode.m_parent = parentNodeID;
          newNode.m_bestChildGrade = 0;

          const Point point(x + m_bbox.left(), y + m_bbox.top());
          newNode.m_point = point;

          PolygonFinder finder(m_graph, layerOwner, ownerPrevPoints);

          const PolyList& polygons = finder(point);
          
          newNode.m_capturedPointsCount = findCapturedPoints(previousPoints,
                                                             layerOwner,
                                                             polygons);

          NodeInfo& parentNode = m_nodes[parentNodeID];
          // If it's a human layer we choose the worse case
          if (parentNode.m_bestChildGrade > newNode.m_capturedPointsCount)
            m_nodes.pop_back();
          else
            parentNode.m_bestChildGrade = newNode.m_capturedPointsCount;
        }
      }

      // Not found any elements for this layer
      if (layerStart == m_nodes.size())
        continue;

      // Invert grades for human layers
      if (isHumanLayer)
        m_nodes[parentNodeID].m_bestChildGrade *= -1;

      m_leafs.reserve(m_leafs.size() + m_nodes.size() - layerStart);
      
      // Last layer
      if (currentLayer == m_depth || (m_numPointsOnBoard + currentLayer == m_maxNumPoints))
      {
        for (std::size_t i = layerStart; i < m_nodes.size(); ++i)
          m_leafs.push_back(i);
        
        continue;
      }

      for (std::size_t i = layerStart; i < m_nodes.size(); ++i)
      {
        if (m_nodes[i].m_capturedPointsCount == 0)
          tasks.push(i);
        else
          m_leafs.push_back(i);
      }
    }
    
    // Update all weights starting from leafs
    
    int bestGrade = 0;
    int prevParentID = -1;
    for (const int id : m_leafs)
    {
      const NodeInfo& node = m_nodes[id];
      if (node.m_parent != prevParentID)
      {
        if (prevParentID != -1)
          m_nodes[prevParentID].m_bestChildGrade += bestGrade;

        bestGrade = node.m_bestChildGrade;
        prevParentID = node.m_parent;
        continue;
      }

      if (prevParentID % 2 == 0) // Current is max/ai layer
      {
        if (bestGrade < node.m_bestChildGrade)
          bestGrade = node.m_bestChildGrade;
      }
      else // Current is min/human layer
      {
        if (bestGrade > node.m_bestChildGrade)
          bestGrade = node.m_bestChildGrade;
      }
    }

    qDebug() << "Decision Tree Final Size:" << m_nodes.size();

    // Decisions are positioned in the first layer
    // TODO: reserve
    for (int i = 1; m_nodes[i].m_parent == 0; ++i)
    {
      points.push_back(m_nodes[i].m_point);
      grades.push_back(m_nodes[i].m_bestChildGrade);
    }
  }
  
  void DecisionTree::findPreviousPoints(int lastPointID,
                                        TPreviousPoints& previousPoints) const
  {
    const NodeInfo *lastNode = &m_nodes[lastPointID];
    
    while(lastNode->m_point.m_x != -1)
    {
      if (lastNode->m_layer % 2 == 1)
        previousPoints.m_ai.push_back(lastNode->m_point);
      else
        previousPoints.m_human.push_back(lastNode->m_point);
      
      lastNode = &m_nodes[lastNode->m_parent];
    }
  }
  
  namespace
  {
    bool isNotIn(const std::vector<Point>& v, const Point& point)
    {
      for (const Point& p : v)
        if (p == point)
          return false;
      
      return true;
    }
  }
  
  int DecisionTree::findCapturedPoints(const TPreviousPoints& previousPoints,
                                       Owner currentOwner,
                                       const PolyList& polygons) const
  {
    if (polygons.empty())
        return 0;
      
    int numPoints = 0;
    
    const std::vector<Point>& opponentPrevPoints = currentOwner == m_ai
        ? previousPoints.m_human
        : previousPoints.m_ai;

    for (int x = m_bbox.left(); x <= m_bbox.right(); ++x)
    {
      for (int y = m_bbox.top(); y <= m_bbox.bottom(); ++y)
      {
        const Point point(x, y);
        
        // We can capture only noncaptured points
        if (m_graph[point].isCaptured())
          continue;
        
        const Owner pointOwner = m_graph[point].owner();
        
        // We can capture only oponent's points
        if (pointOwner == currentOwner)
          continue;
        
        if (pointOwner == Owner::NONE && isNotIn(opponentPrevPoints, point))
          continue;
          
        for (const Polygon_ptr& polygon : polygons)
        {
          if (polygon->contains(point))
          {
            ++numPoints;
            break;
          }
        }
      }
    }
    
    return numPoints;
  }
  
  namespace
  {
    bool isNotPreviousPoint(const TPreviousPoints& previousPoints, const Point& point)
    {
      for (const Point& pai : previousPoints.m_ai)
        if (pai == point)
          return false;
      
      for (const Point& phuman : previousPoints.m_human)
        if (phuman == point)
          return false;
        
      return true;
    }
  }
  
  void DecisionTree::buildAllowedPointsMap(const TPreviousPoints& previousPoints,
                                           std::vector<VectorB>& allowedPoints) const
  {
    for (int x = 0; x < m_bbox.width(); ++x)
    {
      for (int y = 0; y < m_bbox.height(); ++y)
      {
        const Point point(x + m_bbox.left(), y + m_bbox.top());
        
        allowedPoints[x][y] = isNotPreviousPoint(previousPoints, point)
              && m_graph[point].owner() == Owner::NONE
              && !m_graph[point].isCaptured();
      }
    }
  }
}
}