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
    for (auto itr = polygon.begin(), itrEnd = polygon.end();
        itr != itrEnd; ++itr)
    {
      res += (itr->m_x - prevPoint.m_x) * (itr->m_y + prevPoint.m_y); 
      prevPoint = *itr;
    } 
    
    return std::abs(res);
  }
}
  
int KDots::Polygon::area() const
{
  if (m_area < 0)
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

KDots::Point KDots::Polygon::getPrevPoint(std::vector<KDots::Point>::const_iterator current) const
{
  const int currentY = current->m_y;
  for (auto prev = current;;)
  {
    if (prev == m_points.begin())
      prev = --m_points.end();
    else
      --prev;
    
    if(prev->m_y != currentY)
      return *prev;
  }
}

KDots::Point KDots::Polygon::getNextPoint(int& shift, std::vector<KDots::Point>::const_iterator current) const
{
  const int currentY = current->m_y;
  shift = 0;
  for (auto next = current;;)
  {
    ++shift;
    if (next == --m_points.end())
      next = m_points.begin();
    else
      ++next;
    
    if (next->m_y != currentY)
      return *next;
  }
}
  
bool KDots::Polygon::contains(const Point& point) const
{
    // k - a count of points in the same line with "point" object
    // i - crosses count
    int i = 0, shift;

    std::vector<KDots::Point>::const_iterator itr = m_points.begin(), itrEnd = m_points.end();
    while (itr != itrEnd)
    {
      if (itr->m_y != point.m_y)
      {
        ++itr;
        continue;
      }
      
      if (itr->m_x == point.m_x)
        return true;  

      const Point& prevPoint = getPrevPoint(itr);
      const Point& nextPoint = getNextPoint(shift, itr);

      if (itr->m_x < point.m_x && prevPoint.m_y != nextPoint.m_y && shift == 1)
        ++i;
      
      ++itr;
    }

    return i % 2;
}