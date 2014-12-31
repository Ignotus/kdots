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
#include "point.hpp"
#include <QPoint>
#include <QPointF>

namespace KDots
{
  Point::Point()
    : m_x(-1)
    , m_y(-1)
  {}
  
  Point::Point(int x, int y)
    : m_x(x)
    , m_y(y)
  {
  }
  
  Point Point::operator*(int val) const
  {
    return Point(m_x * val, m_y * val);
  }
  
  Point Point::operator+(int val) const
  {
    return Point(m_x + val, m_y + val);
  }
  
  Point Point::operator-(const Point& point) const
  {
    return Point(m_x - point.m_x, m_y - point.m_y);
  }
  
  Point Point::operator+(const Point& point) const
  {
    return Point(m_x + point.m_x, m_y + point.m_y);
  }
  
  bool Point::operator<(const Point& other) const
  {
    return m_x < other.m_x && m_y < other.m_y;
  }
  
  bool Point::operator<=(const Point& other) const
  {
    return m_x <= other.m_x && m_y <= other.m_y;
  }
  
  bool Point::operator>(const Point& other) const
  {
    return m_y > other.m_y && m_x > other.m_x;
  }
  
  bool Point::operator>=(const Point& other) const
  {
    return m_y >= other.m_y && m_x >= other.m_x;
  }

  bool Point::operator==(const Point& a) const
  {
    return m_x == a.m_x && m_y == a.m_y;
  }

  bool Point::operator!=(const Point& a) const
  {
    return !(*this == a);
  }

  bool Point::empty() const
  {
    return m_x == -1 || m_y == -1;
  }
  
  Point::operator QPointF() const
  {
    return QPointF(m_x, m_y);
  }
  
  Point::operator QPoint() const
  {
    return QPoint(m_x, m_y);
  }
  
  QDataStream& operator<<(QDataStream& out, const KDots::Point& obj)
  {
    out << (quint32) obj.m_x << (quint32) obj.m_y;
    return out;
  }

  QDataStream& operator>>(QDataStream& in, KDots::Point& obj)
  {
    in >> (quint32&) obj.m_x >> (quint32&) obj.m_y;
    return in;
  }
}