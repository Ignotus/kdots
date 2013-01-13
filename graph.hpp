/*
 * KDots
 * Copyright (c) 2011-2012 Minh Ngo <nlminhtl@gmail.com>
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
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef KDOTS_GRAPH_HPP
#define KDOTS_GRAPH_HPP
#include <vector>
#include <iterator>
#include "graphpoint.hpp"

namespace KDots {
  class Graph;
  
  template<class A>
  class graph_iterator;
  
  class KDOTS_EXPORT Graph {
      std::vector<std::vector<GraphPoint>> m_graph;
    public:
      typedef graph_iterator<GraphPoint> iterator;
      typedef graph_iterator<const GraphPoint> const_iterator;
      
      Graph(int width, int height);
      
      iterator begin();
      const_iterator begin() const;
      
      iterator end();
      const_iterator end() const;
      
      bool isValid(const Point& point) const {
        return point > Point() && point < Point(width(), height());
      }
      
      std::size_t width() const {
        return m_graph.size();
      }
      
      std::size_t height() const {
        return m_graph.front().size();
      }
      
      GraphPoint& operator[](const Point& index) {
        return m_graph[index.x()][index.y()];
      }
      
      const GraphPoint& operator[](const Point& index) const {
        return m_graph[index.x()][index.y()];
      }
      
      std::vector<GraphPoint>& operator[](int index) {
        return m_graph[index];
      }
      
      const std::vector<GraphPoint>& operator[](int index) const {
        return m_graph[index];
      }
      
      void addEdge(const KDots::Point& first, const KDots::Point& second);
      void removeEdge(const Point& first, const Point& second);
  };
  
  template<class A>
  class graph_iterator : public std::iterator<std::forward_iterator_tag, A> {
      Graph *m_graph;
      std::size_t m_x, m_y;
    public:
    
      Point point() const {
        return Point(m_x, m_y);
      }
      
      graph_iterator()
        : m_graph(NULL)
        , m_x(0)
        , m_y(0) {
      }
      
      graph_iterator(Graph *graph, std::size_t x = 0, std::size_t y = 0)
        : m_graph(graph)
        , m_x(x)
        , m_y(y) {
      }
      
      graph_iterator(const Graph *graph, std::size_t x = 0, std::size_t y = 0)
        : m_graph(const_cast<Graph *>(graph))
        , m_x(x)
        , m_y(y) {
      }
      
      graph_iterator(const graph_iterator& other)
        : m_graph(other.m_graph)
        , m_x(other.m_x)
        , m_y(other.m_y) {
      }
      
      graph_iterator& operator= (const graph_iterator& other) {
        m_graph = other.m_graph;
        m_x = other.m_x;
        m_y = other.m_y;
        return *this;
      }
      
      bool operator== (const graph_iterator& other) const {
        return m_graph == other.m_graph && m_x == other.m_x && m_y == other.m_y;
      }
      
      bool operator!= (const graph_iterator& other) const {
        return m_graph != other.m_graph || m_x != other.m_x || m_y != other.m_y;
      }
      
      A& operator* () {
        return (*m_graph)[m_x][m_y];
      }
      
      A *operator-> () {
        return &(*m_graph)[m_x][m_y];
      }
      
      graph_iterator& operator++ () {
        if (m_x + 1 == m_graph->width()) {
          m_x = 0;
          ++m_y;
        } else
          ++m_x;
          
        return *this;
      }
      
      graph_iterator operator++ (int) {
        graph_iterator res(*this);
        ++(*this);
        return res;
      }
  };
}

#endif
