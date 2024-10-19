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
#include "decisiontree.hpp"

#include <boardmodel.hpp>
#include <constants.hpp>
#include <graph.hpp>
#include <polygonfinder.hpp>
#include <stepqueue.hpp>
#include <iboardview.hpp>

#include <stack>
#include <ctime>
#include <cassert>

#include <boost/range/combine.hpp>

#include <QDebug>
#include <KgDifficulty>

namespace KDots
{
namespace simpleai
{
  namespace
  {
    const std::map<KgDifficultyLevel::StandardLevel, int> DIFFICULTY_TO_DEPTH = {
      {KgDifficultyLevel::Easy, 2},
      {KgDifficultyLevel::Medium, 3},
      {KgDifficultyLevel::Hard, 4}
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
    qDebug() << "m_depth" << m_depth;
  }

  void Rival::onPointAdded(const QPoint& point)
  {
    Q_UNUSED(point);
    if (m_board->stepQueue().getCurrentOwner() == m_human)
      return;

    addPoint();
  }

  bool Rival::addRandomPoint()
  {
    const Graph& graph = m_board->graph();
    const QPoint newPoint(rand() % graph.width(), rand() % graph.height());
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

    auto match = [&](const QPoint& current, const MapData& data) -> bool {
      auto checkPoint = [&](const QPoint& offset, MapElement value) -> bool {
        const QPoint newPoint(current + offset);

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

      const QPoint& currentMarker = data.m_current;
      for (int row = 0; row < data.m_map.size(); ++row)
      {
        const MapLine& line = data.m_map[row];
        for (int column = 0; column < line.size(); ++column)
        {
          if (!checkPoint({column - currentMarker.x(), row - currentMarker.y()}, line[column]))
            return false;
        }
      }
      return true;
    };

    auto cellPriority = [&](const QPoint& current) -> float {
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

    std::vector<VectorF> importanceMatrix(bb.width(), VectorF(bb.height()));

    const QPoint& offsetPoint = bb.topLeft();
    for (int x = 0; x < bb.width(); ++x)
    {
      for (int y = 0; y < bb.height(); ++y)
      {
        const QPoint graphPoint(QPoint(x, y) + offsetPoint);
        if (graph[graphPoint].owner() != Owner::NONE)
          importanceMatrix[x][y] = -std::numeric_limits<float>::infinity();
        else
          importanceMatrix[x][y] = cellPriority(graphPoint);
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

  void Rival::addPoint(bool random)
  {
    const QRect bbox(getBoundingBox());

    const Graph& graph = m_board->graph();
    
    const int pointsOnBoard = m_board->stepQueue().getAllPoints().size();
    
    DecisionTree tree(graph, bbox, pointsOnBoard, m_depth, m_ai);
    
    std::vector<QPoint> decisions;
    std::vector<float> decisionGrades;
    
    {
    const int begin = std::time(0);
    tree.calculateDecisions(decisions, decisionGrades);
    qDebug() << "Decision making time:" << (std::time(0) - begin);
    }
    
    qDebug() << "Number of decisions founded:" << decisions.size();

    if (!decisions.empty())
    {
      const auto importanceMatrix(getImportanceMatrix(bbox));
      for (const boost::tuple<const QPoint&, float&>& pair : boost::combine(decisions, decisionGrades))
      {
        const QPoint p = pair.get<0>() - bbox.topLeft();
        assert(p.x() >= 0);
        assert(p.y() >= 0);
        assert(p.x() < importanceMatrix.size());
        assert(p.y() < importanceMatrix[0].size());

        // Heuristics will contribute only if decision tree cannot decide on the best move
        (pair.get<1>() *= m_k2) += m_k1 * importanceMatrix[p.x()][p.y()];
      }

      const int pointID = std::distance(decisionGrades.begin(),
                                        std::max_element(decisionGrades.begin(),
                                                         decisionGrades.end()));

      assert(pointID >= 0 && pointID < decisions.size());
      emit needAddPoint(decisions[pointID]);
    }
    // Else -> End of the game
  }

  void Rival::setBoardModel(const BoardModel *board)
  {
    m_board = board;

    if (m_board->stepQueue().getCurrentOwner() == m_ai)
    {
      addPoint(true);
    }
  }
}
}

#include "moc_rival.cpp"
