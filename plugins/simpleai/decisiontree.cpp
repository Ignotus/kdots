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
#include <memory>

#include <QRect>
#include <QDebug>
#include <graph.hpp>
#include <stepqueue.hpp>
#include <polygonfinder.hpp>

namespace KDots
{
namespace simpleai
{
  DecisionTree::DecisionTree(const Graph& graph, const QRect& bbox,
                             int numPointsOnBoard, int depth, Owner ai)
    : m_graph(graph)
    , m_bbox(bbox)
    , m_numPointsOnBoard(numPointsOnBoard)
    , m_maxNumPoints(graph.width() * graph.height())
    , m_depth(depth*2)
    , m_ai(ai)
    , m_human(ai == Owner::FIRST ? Owner::SECOND : Owner::FIRST)
  {
    qDebug() << "Bounding box:" << bbox.width() << bbox.height();
    qDebug() << "DecisionTee {depth =" << m_depth << "}";
  }

  namespace
  {
      bool contains(const std::vector<QPoint>& v, const QPoint& p)
      {
          return std::find(v.begin(), v.end(), p) != v.end();
      }
  }

  int DecisionTree::gradeMove(const QPoint& newPoint,
                              std::vector<QPoint>& decisionStack,
                              std::vector<QPoint>& humanDecisionStack,
                              int depth) const
  {
    const Owner currentPlayer = depth % 2 == 0 ? m_ai : m_human;
    const Owner otherPlayer = currentPlayer == m_ai ? m_human : m_ai;

    PolygonFinder finder(
      m_graph,
      currentPlayer,
      depth % 2 == 0 ? decisionStack : humanDecisionStack
    );

    const PolyList& polygons = finder(newPoint);

    if (polygons.size() > 0) {
      for (const auto& polygonPtr : polygons) {
        if (anyPointsCaptured(*polygonPtr,
                              otherPlayer == m_ai ? decisionStack : humanDecisionStack,
                              otherPlayer)) {

          const int score = m_depth - depth;
          return currentPlayer == m_ai ? score : -score;
        }
      }
    }

    if (depth + 1 == m_depth) {
      return 0;
    }

    if (currentPlayer == m_ai) {
      decisionStack.push_back(newPoint);
    } else {
      humanDecisionStack.push_back(newPoint);
    }

    // Limit a search box to improve speed
    QRect bbox(
      std::max(newPoint.x() - 2, 0),
      std::max(newPoint.y() - 2, 0),
      std::min<int>(newPoint.x() + 2, m_graph.width() - 1),
      std::min<int>(newPoint.y() + 2, m_graph.height() - 1)
    );

    bbox &= m_bbox;

    int worseScore = 0;
    int bestScore = 0;

    for (int x = bbox.x(); x <= bbox.right(); ++x) {
      for (int y = bbox.y(); y <= bbox.bottom(); ++y) {
        const QPoint& currentPoint = QPoint{x, y};
        const auto& p = m_graph[currentPoint];
        if (p.owner() != Owner::NONE)
          continue;

        if (contains(decisionStack, currentPoint))
          continue;

        if (contains(humanDecisionStack, currentPoint))
          continue;

        const double r = ((double) rand() / (RAND_MAX)) + 1;

        // Drops 75% of the decision tree to improve speed
        if (r < 0.75)
          continue;

        const int currentScore = gradeMove(
          currentPoint,
          decisionStack,
          humanDecisionStack,
          depth + 1
        );

        worseScore = std::min(currentScore, worseScore);
        bestScore = std::max(currentScore, bestScore);
      }
    }

    if (currentPlayer == m_ai) {
      decisionStack.pop_back();
    } else {
      humanDecisionStack.pop_back();
    }

    if (currentPlayer == m_ai) {
      // Max step
      return bestScore;
    } else {
      // Min step
      return worseScore;
    }
  }

  bool DecisionTree::anyPointsCaptured(const Polygon& polygon,
                                       const std::vector<QPoint>& otherPoints,
                                       Owner other) const
  {
    for (int x = m_bbox.x(); x <= m_bbox.right(); ++x) {
      for (int y = m_bbox.y(); y <= m_bbox.bottom(); ++y) {
        const QPoint& qp = QPoint{x, y};
        const auto& p = m_graph[qp];

        if (p.isCaptured())
          continue;

        if (p.owner() != other && !contains(otherPoints, qp))
          continue;

        if (polygon.contains(qp)) {
          qDebug() << "p = {" << x << y << "} could be captured by"
                   << (other == m_ai ? "Human" : "AI") << "after"
                   << otherPoints.size() << "moves";
          return true;
        }
      }
    }

    return false;
  }


  void DecisionTree::calculateDecisions(std::vector<QPoint>& oPoints,
                                        VectorF& oWeights)
  {
    qDebug() << "x" << m_bbox.x() << m_bbox.right();
    qDebug() << "y" << m_bbox.y() << m_bbox.bottom();

    std::vector<QPoint> decisionStack;
    std::vector<QPoint> otherPlayerDecisionStack;

    for (int x = m_bbox.x(); x <= m_bbox.right(); ++x) {
      for (int y = m_bbox.y(); y <= m_bbox.bottom(); ++y) {
        const auto& p = m_graph[QPoint{x, y}];

        if (p.owner() != Owner::NONE)
          continue;

        decisionStack.clear();
        otherPlayerDecisionStack.clear();

        const int score = gradeMove(
          {x, y},
          decisionStack,
          otherPlayerDecisionStack,
          0);

        qDebug() << "p = {" << x << y << "} score" << score;

        oPoints.push_back({x, y});
        oWeights.push_back(score);
      }
    }
  }

}
}
