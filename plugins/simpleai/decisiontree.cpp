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
    , m_depth(depth)
    , m_ai(ai)
  {
    qDebug() << "Bounding box:" << bbox.width() << bbox.height();
  }

  void DecisionTree::calculateDecisions(std::vector<QPoint>& oPoints,
                                        VectorF& oWeights)
  {
    const int w = m_graph.width();
    const int h = m_graph.height();
    qDebug() << "x" << m_bbox.x() << m_bbox.right();
    qDebug() << "y" << m_bbox.y() << m_bbox.bottom();
    for (int x = m_bbox.x(); x <= m_bbox.right(); ++x) {
      for (int y = m_bbox.y(); y <= m_bbox.bottom(); ++y) {
        const auto& p = m_graph[QPoint{x, y}];
        qDebug() << "x" << x << "y" << y << p.owner();
        if (p.owner() != Owner::NONE)
          continue;

        oPoints.push_back({x, y});
        oWeights.push_back(((double)rand()) / RAND_MAX);
      }
    }
  }
  
}
}
