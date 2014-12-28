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
    m_graph[first.x()][first.y()].edges().addEdge(second);
    m_graph[second.x()][second.y()].edges().addEdge(first);
  }

  void Graph::removeEdge(const Point& first, const Point& second)
  {
    m_graph[first.x()][first.y()].edges().removeEdge(second);
    m_graph[second.x()][second.y()].edges().removeEdge(first);
  }
}
