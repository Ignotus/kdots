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
#ifndef KDOTS_EDGELIST_HPP
#define KDOTS_EDGELIST_HPP
#include <stdexcept>
#include "point.hpp"

namespace KDots
{
  template<int SIZE>
  class KDOTS_EXPORT EdgeList final
  {
    int m_count;
    Point m_pointList[SIZE];
  public:
    EdgeList()
      : m_count(0)
    {}

    bool addEdge(const Point& point)
    {
      if(m_count == SIZE || hasPoint(point))
        return false;

      m_pointList[m_count++] = point;
      return true;
    }

    int size() const
    {
      return m_count;
    }

    bool hasPoint(const Point& point)
    {
      for(int i = 0;  i < m_count; ++i)
      {
        if(m_pointList[i] == point)
          return true;
      }

      return false;
    }

    Point& operator[](int index)
    {
      return const_cast<EdgeList<SIZE>&>(static_cast<const EdgeList<SIZE>&>(*this) [index]);
    }

    const Point& operator[](int index) const
    {
      if(index < 0 || index >= m_count)
        throw std::runtime_error("beyond the limit of the array");
      return m_pointList[index];
    }

    bool removeEdge(const Point& toPoint)
    {
      for(int i = 0; i < m_count; ++i)
      {
        if(m_pointList[i] == toPoint)
        {
          m_count--;

          if(i != m_count)
            m_pointList[i] = m_pointList[m_count];

          return true;
        }
      }

      return false;
    }
  };
}

#endif
