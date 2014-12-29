/*
 * KDots
 * Copyright(c) 2011-2012, 2014 Minh Ngo <nlminhtl@gmail.com>
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

#include "polygon.hpp"

KDots::Polygon::Polygon()
  : m_filled(false)
  , m_area(-1)
{
}

KDots::Polygon::Polygon(const std::vector<Point>& points)
  : m_points(points)
  , m_filled(false)
  , m_area(-1)
{
}

namespace
{
  int doubleArea(const std::vector<KDots::Point>& polygon)
  {
    int res = 0;
    KDots::Point prevPoint = polygon.back();
    for(auto itr = polygon.begin(), itrEnd = polygon.end();
        itr != itrEnd; ++itr)
    {
      res += (itr->x() - prevPoint.x()) * (itr->y() + prevPoint.y()); 
      prevPoint = *itr;
    } 
    
    return std::abs(res);
  }
}
  
int KDots::Polygon::area() const
{
  if(m_area < 0)
  {
    m_area = doubleArea(m_points);
  }

  return m_area;
}

const std::vector<KDots::Point>& KDots::Polygon::points() const
{
  return m_points;
}

bool KDots::Polygon::isFilled() const
{
  return m_filled;
}

void KDots::Polygon::setFilled(bool filled)
{
  m_filled = filled;
}

KDots::Owner KDots::Polygon::owner() const
{
  return m_owner;
}

void KDots::Polygon::setOwner(Owner own)
{
  m_owner = own;
}