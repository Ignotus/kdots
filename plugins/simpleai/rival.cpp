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
#include "rival.hpp"
#include "prioritymap.hpp"

#include <boardmodel.hpp>
#include <constants.hpp>
#include <graph.hpp>
#include <point.hpp>
#include <polygonfinder.hpp>
#include <stepqueue.hpp>

#include <stack>

#include <KDebug>
#include <KgDifficulty>

namespace KDots
{
  namespace simpleai
  {
    namespace
    {
      const std::map<KgDifficultyLevel::StandardLevel, int> DIFFICULTY_TO_DEPTH = {
        {KgDifficultyLevel::Easy, 2},
        {KgDifficultyLevel::Medium, 4},
        {KgDifficultyLevel::Hard, 6}
      };
    }

    Rival::Rival(QObject *parent)
      : IRival(parent)
      , m_board(nullptr)
      , m_human(Owner::FIRST)
      , m_ai(Owner::SECOND)
      , m_depth(DIFFICULTY_TO_DEPTH.at(KgDifficultyLevel::Medium))
      , m_k1(1)
      , m_k2(1)
    {
      PriorityMap::instance();
      Kg::difficulty()->setEditable(true);
    }

    Owner Rival::owner() const
    {
      return m_ai;
    }

    void Rival::onDifficultyChanged(const KgDifficultyLevel *level)
    {
      m_depth = DIFFICULTY_TO_DEPTH.at(level->standardLevel());
    }

    void Rival::onPointAdded(const Point& point)
    {
      Q_UNUSED(point);
      if (m_board->stepQueue().getCurrentOwner() == m_human)
        return;

      addPoint();
    }

    bool Rival::addRandomPoint()
    {
      const Graph& graph = m_board->graph();
      const Point newPoint(rand() % graph.width(), rand() % graph.height());
      if (graph[newPoint].owner() == Owner::NONE)
      {
        emit needAddPoint(newPoint);
        return true;
      }

      return false;
    }

    std::vector<Rival::VectorF> Rival::getImportanceMatrix(const QRect& bb) const
    {
      const Graph& graph = m_board->graph();

      auto match = [&](const Point& current, const MapData& data) -> bool {
        auto checkPoint = [&](const Point& offset, MapElement value) -> bool {
          const Point newPoint(current + offset);

          if (!graph.isValid(newPoint))
            return false;

          const GraphPoint& graphPoint = graph[newPoint];
          switch (value)
          {
          case MapElement::EM:
            return graphPoint.owner() == Owner::NONE;
          case MapElement::FI:
            return graphPoint.owner() == Owner::FIRST;
          case MapElement::PF:
            return graphPoint.owner() & (Owner::NONE | Owner::FIRST);
          case MapElement::PS:
            return graphPoint.owner() & (Owner::NONE | Owner::SECOND);
          case MapElement::SE:
            return graphPoint.owner() == Owner::SECOND;
          default:
            return true;
          }
        };

        const Point& currentMarker = data.m_current;
        for (int row = 0; row < data.m_map.size(); ++row)
        {
          const MapLine& line = data.m_map[row];
          for (int column = 0; column < line.size(); ++column)
          {
            if (!checkPoint({column - currentMarker.m_x, row - currentMarker.m_y}, line[column]))
              return false;
          }
        }
        return true;
      };

      auto cellPriority = [&](const Point& current) -> float {
        float maxPriority = -std::numeric_limits<float>::infinity();

        for (const MapData& data : PriorityMap::instance().priorityMap())
        {
          if (match(current, data))
              maxPriority = std::max(maxPriority, data.m_priority);
        }

        if (maxPriority == -std::numeric_limits<float>::infinity())
          maxPriority = 0;

        return maxPriority;
      };

      kDebug() << "BB size:" << bb.width() << bb.height();
      std::vector<VectorF> importanceMatrix(bb.width(), VectorF(bb.height()));

      const QPoint& offsetPoint = bb.topLeft();
      kDebug() << "Offset:" << offsetPoint;
      for (int x = 0; x < bb.width(); ++x)
      {
        for (int y = 0; y < bb.height(); ++y)
        {
          if (graph[x + offsetPoint.x()][y + offsetPoint.y()].owner() != Owner::NONE)
            importanceMatrix[x][y] = -std::numeric_limits<float>::infinity();
          else
          {
            importanceMatrix[x][y] = cellPriority({offsetPoint.x() + x, offsetPoint.y() + y});
            kDebug() << "(" << (offsetPoint.x() + x)
                     << "," << (offsetPoint.y() + y)
                     << ")" << importanceMatrix[x][y];
          }
        }
      }

      return importanceMatrix;
    }

    QRect Rival::getBoundingBox() const
    {
      const Graph& graph = m_board->graph();
      std::size_t min_x = graph.width() - 1;
      std::size_t max_x = 0;

      std::size_t min_y = graph.height() - 1;
      std::size_t max_y = 0;

      for (std::size_t x = 0; x < graph.width(); ++x)
      {
        for (std::size_t y = 0; y < graph.height(); ++y)
        {
          const GraphPoint& p = graph[x][y];
          if (p.owner() == Owner::NONE)
            continue;

          min_x = std::min(min_x, x);
          max_x = std::max(max_x, x);
          min_y = std::min(min_y, y);
          max_y = std::max(max_y, y);
        }
      }

      if (min_x > 0)
        --min_x;

      if (max_x < graph.width() - 1)
        ++max_x;

      if (min_y > 0)
        --min_y;

      if (max_y < graph.height() - 1)
        ++max_y;

      return QRect(QPoint(min_x, min_y), QPoint(max_x, max_y));
    }

    void Rival::findCapturedPoints(const QRect& bbox,
                                   const std::vector<VectorB>& allowedPoints,
                                   const PolyList& polygons,
                                   std::unordered_set<Point>& capturedPoints) const
    {
      if (polygons.empty())
        return;

      for (int x = bbox.left(); x <= bbox.right(); ++x)
      {
        for (int y = bbox.top(); y <= bbox.bottom(); ++y)
        {
          if (!allowedPoints[x - bbox.left()][y - bbox.top()])
            continue;

          const Point point(x, y);
          for (const Polygon_ptr& polygon : polygons)
          {
            if (polygon->contains(point))
            {
              capturedPoints.insert(point);
              break;
            }
          }
        }
      }
    }

    struct NodeInfo
    {
      int m_parent; // Index from the vector
      int m_layer;
      int m_bestChildGrade;
      int m_capturedPointsCount;

      Point m_point;
      std::unordered_set<Point> m_capturedPoints;
    };

    void Rival::findPreviousPoints(const std::vector<NodeInfo>& decisionTree,
                                   int lastPointID,
                                   std::unordered_set<Point>& previousPoints,
                                   std::unordered_set<Point>& capturedPoints) const
    {
      const NodeInfo *lastNode = &decisionTree[lastPointID];
      //kDebug() << "Last node: {" << lastNode->m_point.m_x << "," << lastNode->m_point.m_y << "}";
      while (lastNode->m_point.m_x != -1)
      {
        previousPoints.insert(lastNode->m_point);
        const std::unordered_set<Point>& lastCapturedPoints = lastNode->m_capturedPoints;
        capturedPoints.insert(lastCapturedPoints.begin(), lastCapturedPoints.end());
        lastNode = &decisionTree[lastNode->m_parent];
      }
    }

    void Rival::buildAllowedPointsMap(const QRect& bbox,
                                      const std::unordered_set<Point>& previousPoints,
                                      std::vector<VectorB>& allowedPoints) const
    {
      const Graph& graph = m_board->graph();
      for (int x = 0; x < bbox.width(); ++x)
      {
        for (int y = 0; y < bbox.height(); ++y)
        {
          const Point point(x + bbox.left(), y + bbox.top());
          allowedPoints[x][y] = !previousPoints.count(point)
              && graph[point].owner() == Owner::NONE
              && !graph[point].isCaptured();
        }
      }
    }

    void Rival::addPoint(bool random)
    {
      if (random)
      {
        while (!addRandomPoint());
        return;
      }

      const QRect bbox(getBoundingBox());

      // For other elements importance is 0
      const auto importanceMatrix(getImportanceMatrix(bbox));

      std::vector<NodeInfo> decisionTree(1);

      {
      NodeInfo& baseNode = decisionTree.back();
      baseNode.m_parent = -1;
      baseNode.m_bestChildGrade = 0;
      baseNode.m_layer = 0;

      baseNode.m_capturedPointsCount = 0;

      baseNode.m_point = {-1, -1};
      }

      std::stack<std::size_t> tasks;
      tasks.push(0);

      const Graph& graph = m_board->graph();

      const int begin = time(0);
      while (!tasks.empty())
      {
        const int parentNodeID = tasks.top();
        tasks.pop();

        std::unordered_set<Point> previousPoints;
        std::unordered_set<Point> capturedPoints;

        findPreviousPoints(decisionTree, parentNodeID,
                           previousPoints, capturedPoints);

        std::vector<VectorB> allowedPoints(bbox.width(), VectorB(bbox.height(), false));
        buildAllowedPointsMap(bbox, previousPoints, allowedPoints);

        const std::size_t layerStart = decisionTree.size();

        for (int x = 0; x < bbox.width(); ++x)
        {
          for (int y = 0; y < bbox.height(); ++y)
          {
            if (!allowedPoints[x][y])
              continue;

            decisionTree.emplace_back();

            const int newNodeID = decisionTree.size() - 1;
            NodeInfo& newNode = decisionTree[newNodeID];
            NodeInfo& parentNode = decisionTree[parentNodeID];

            newNode.m_layer = parentNode.m_layer + 1;
            newNode.m_parent = parentNodeID;
            newNode.m_bestChildGrade = 0;

            const Point point(x + bbox.left(), y + bbox.top());
            newNode.m_point = point;

            PolygonFinder finder(graph,
                                 parentNode.m_layer % 2 == 0 ? m_ai : m_human,
                                 previousPoints);

            const PolyList& polygons = finder(point);

            findCapturedPoints(bbox,
                               allowedPoints,
                               polygons,
                               newNode.m_capturedPoints);

            newNode.m_capturedPointsCount = newNode.m_capturedPoints.size();
            if (parentNode.m_bestChildGrade > newNode.m_capturedPointsCount)
              decisionTree.pop_back();
            else
              parentNode.m_bestChildGrade = newNode.m_capturedPointsCount;
          }
        }

        if (layerStart == decisionTree.size())
          continue;

        NodeInfo& parentNode = decisionTree[parentNodeID];

        if (parentNode.m_layer % 2 == 1) // Human layer
          // Invert grades for human layers
          decisionTree[parentNodeID].m_bestChildGrade *= -1;

        // Last layer
        if (parentNode.m_layer + 1 == m_depth)
          continue;

        for (std::size_t i = layerStart; i < decisionTree.size(); ++i)
        {
          if (decisionTree[i].m_capturedPointsCount == parentNode.m_bestChildGrade)
            tasks.push(i);
        }
      }

      int bestGrade = 0;
      int prevParentID = -1;
      for (auto it = decisionTree.rbegin(), end = --decisionTree.rend(); it != end; ++it)
      {
        if (it->m_parent != prevParentID)
        {
          if (prevParentID != -1)
            decisionTree[prevParentID].m_bestChildGrade += bestGrade;

          bestGrade = it->m_bestChildGrade;
          prevParentID = it->m_parent;
          continue;
        }

        if (prevParentID % 2 == 0) // Current is max/ai layer
        {
          if (bestGrade < it->m_bestChildGrade)
            bestGrade = it->m_bestChildGrade;
        }
        else // Current is min/human layer
        {
          if (bestGrade > it->m_bestChildGrade)
            bestGrade = it->m_bestChildGrade;
        }
      }

      kDebug() << "Best grade (Decision tree): " << bestGrade;
      kDebug() << "Decision Tree Final Size:" << decisionTree.size();

      std::vector<Point> decisions;
      std::vector<float> decisionGrades;
      for (int i = 1; decisionTree[i].m_parent == 0; ++i)
      {
        decisions.push_back(decisionTree[i].m_point);
        decisionGrades.push_back(decisionTree[i].m_bestChildGrade * m_k2);
        kDebug() << decisionTree[i].m_point.m_x << ", " << decisionTree[i].m_point.m_y
                 << ":" << decisionTree[i].m_bestChildGrade * m_k2;
      }

      kDebug() << "Number of decisions founded:" << decisions.size();

      if (!decisions.empty())
      {
        for (std::size_t i = 0; i < decisions.size(); ++i)
        {
          const Point& p = decisions[i];

          decisionGrades[i] += m_k1 * importanceMatrix[p.m_x - bbox.left()][p.m_y - bbox.top()];
        }

        const int pointID = std::distance(decisionGrades.begin(),
                                          std::max_element(decisionGrades.begin(),
                                                          decisionGrades.end()));
        emit needAddPoint(decisions[pointID]);

        kDebug() << "Decision making time:" << (time(0) - begin);
      }
      else // Last points, doesn't matter a lot so :)
      {
        kDebug() << "Empty decisions";
        kDebug() << bbox.left() << bbox.right();
        kDebug() << bbox.top() << bbox.bottom();
        for (int x = bbox.left(); x <= bbox.right(); ++x)
        {
          for (int y = bbox.top(); y <= bbox.bottom(); ++y)
          {
            const Point point(x, y);
            if (graph[point].owner() == Owner::NONE && !graph[point].isCaptured())
            {
              kDebug() << "Add point:" << point.m_x << point.m_y;
              emit needAddPoint(point);
              return;
            }
          }
        }
      }
    }

    void Rival::setBoardModel(BoardModel *board)
    {
      m_board = board;

      if (m_board->stepQueue().getCurrentOwner() == m_ai)
      {
        addPoint(true);
      }
    }
  }
}
