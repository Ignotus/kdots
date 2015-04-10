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

#include "polygon.hpp"
#include "polygon_p.hpp"

#include <QPoint>

namespace KDots
{
  PolygonPrivate::PolygonPrivate(const std::vector<QPoint>& points)
    : m_points(points)
    , m_filled(false)
    , m_area(-1)
  {
  }

  QPoint PolygonPrivate::getPrevPoint(std::vector<QPoint>::const_iterator current) const
  {
    const int currentY = current->y();
    for (auto prev = current;;)
    {
      if (prev == m_points.begin())
        prev = --m_points.end();
      else
        --prev;

      if (prev->y() != currentY)
        return *prev;
    }
  }

  QPoint PolygonPrivate::getNextPoint(int& shift, std::vector<QPoint>::const_iterator current) const
  {
    const int currentY = current->y();
    shift = 0;
    for (auto next = current;;)
    {
      ++shift;
      if (next == --m_points.end())
        next = m_points.begin();
      else
        ++next;

      if (next->y() != currentY)
        return *next;
    }
  }

  //////////////////////////////////////////////////////////////////////////////

  Polygon::Polygon(const std::vector<QPoint>& points)
    : d_ptr(new PolygonPrivate(points))
  {
  }

  Polygon::~Polygon()
  {
  }

  namespace
  {
    int doubleArea(const std::vector<QPoint>& polygon)
    {
      int res = 0;
      QPoint prevPoint = polygon.back();
      for (auto itr = polygon.begin(), itrEnd = polygon.end();
          itr != itrEnd; ++itr)
      {
        res += (itr->x() - prevPoint.x()) * (itr->y() + prevPoint.y());
        prevPoint = *itr;
      }

      return std::abs(res);
    }
  }

  int Polygon::area() const
  {
    Q_D(const Polygon);
    if (d->m_area < 0)
    {
      d->m_area = doubleArea(d->m_points);
    }

    return d->m_area;
  }

  const std::vector<QPoint>& Polygon::points() const
  {
    Q_D(const Polygon);
    return d->m_points;
  }

  bool Polygon::isFilled() const
  {
    Q_D(const Polygon);
    return d->m_filled;
  }

  void Polygon::setFilled(bool filled)
  {
    Q_D(Polygon);
    d->m_filled = filled;
  }

  Owner Polygon::owner() const
  {
    Q_D(const Polygon);
    return d->m_owner;
  }

  void Polygon::setOwner(Owner own)
  {
    Q_D(Polygon);
    d->m_owner = own;
  }

  bool Polygon::contains(const QPoint& point) const
  {
    Q_D(const Polygon);
    // k - a count of points in the same line with "point" object
    // i - crosses count
    int i = 0, shift;

    auto itr = d->m_points.begin();
    auto itrEnd = d->m_points.end();
    while (itr != itrEnd)
    {
      if (itr->y() != point.y())
      {
        ++itr;
        continue;
      }

      if (itr->x() == point.x())
        return true;

      const QPoint& prevPoint = d->getPrevPoint(itr);
      const QPoint& nextPoint = d->getNextPoint(shift, itr);

      if (itr->x() < point.x() && prevPoint.y() != nextPoint.y() && shift == 1)
        ++i;

      ++itr;
    }

    return i % 2;
  }
}
