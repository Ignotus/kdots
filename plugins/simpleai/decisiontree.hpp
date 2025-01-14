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
#include <QPoint>

#include <constants.hpp>
#include <polygon.hpp>

class QRect;

namespace KDots
{
class Graph;
namespace simpleai
{
  typedef std::vector<float> VectorF;
  
  class DecisionTree
  {
  public:
    /* ai : Owner - identified in the graph as a point made by AI.
     * bbox : QRect - decision should be made within the bounding box
     * depth : int - Depth of the search tree
     */
    DecisionTree(const Graph& graph, const QRect& bbox,
                 int numPointsOnBoard, int depth, Owner ai);
    
    void calculateDecisions(std::vector<QPoint>& oPoints, VectorF& oWeights);

  private:
    int gradeMove(const QPoint& newPoint,
                  std::vector<QPoint>& decisionStack,
                  std::vector<QPoint>& humanDecisionStack,
                  int depth) const;

    bool anyPointsCaptured(const Polygon& polygon,
                           const std::vector<QPoint>& otherPoints,
                           Owner other) const;
  
  private:
    const Graph& m_graph;
    const QRect& m_bbox;
    const int m_numPointsOnBoard;
    const int m_maxNumPoints;
    const int m_depth;
    const Owner m_ai;
    const Owner m_human;
  };
}
}
