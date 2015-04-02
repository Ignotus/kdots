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
#include "graph.hpp"

namespace KDots
{
  Graph::Graph(int width, int height)
    : m_graph(width, std::vector<GraphPoint>(height))
  {
  }
  
  Graph::iterator Graph::begin()
  {
    return iterator(this);
  }
    
  Graph::const_iterator Graph::begin() const
  {
    return const_iterator(this);
  }
    
  Graph::iterator Graph::end()
  {
    iterator itr(this, 0, height());
    return itr;
  }
    
  Graph::const_iterator Graph::end() const
  {
    const_iterator itr(this, 0, height());
    return itr;
  }

  void Graph::addEdge(const Point& first, const Point& second)
  {
    m_graph[first.m_x][first.m_y].edges().addEdge(second);
    m_graph[second.m_x][second.m_y].edges().addEdge(first);
  }

  void Graph::removeEdge(const Point& first, const Point& second)
  {
    m_graph[first.m_x][first.m_y].edges().removeEdge(second);
    m_graph[second.m_x][second.m_y].edges().removeEdge(first);
  }
  
  bool Graph::isValid(const Point& point) const
  {
    return point > Point() && point < Point(width(), height());
  }

  std::size_t Graph::width() const
  {
    return m_graph.size();
  }

  std::size_t Graph::height() const
  {
    return m_graph.front().size();
  }

  GraphPoint& Graph::operator[](const Point& index)
  {
    return m_graph[index.m_x][index.m_y];
  }

  const GraphPoint& Graph::operator[](const Point& index) const
  {
    return m_graph[index.m_x][index.m_y];
  }

  std::vector<GraphPoint>& Graph::operator[](int index)
  {
    return m_graph[index];
  }

  const std::vector<GraphPoint>& Graph::operator[](int index) const
  {
    return m_graph[index];
  }
}
