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
#include <stdexcept>

#include <QPoint>

namespace KDots
{
  template<int SIZE>
  class EdgeList final
  {
  public:
    EdgeList()
      : m_count(0)
    {}

    bool addEdge(const QPoint& point)
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

    bool hasPoint(const QPoint& point)
    {
      return std::any_of(m_pointList, &m_pointList[m_count], [&point](const QPoint& p) {
        return p == point;
      });
    }

    QPoint& operator[](int index)
    {
      return const_cast<EdgeList<SIZE>&>(static_cast<const EdgeList<SIZE>&>(*this) [index]);
    }

    const QPoint& operator[](int index) const
    {
      if(index < 0 || index >= m_count)
        throw std::runtime_error("beyond the limit of the array");
      return m_pointList[index];
    }

    bool removeEdge(const QPoint& toPoint)
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
  
  private:
    int m_count;
    QPoint m_pointList[SIZE];
  };
}
