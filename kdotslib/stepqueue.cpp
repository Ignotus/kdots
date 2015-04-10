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
#include "stepqueue.hpp"

#include <QDebug>

namespace KDots
{
  StepQueue::StepQueue(Owner firstPlayer)
    : m_owner(firstPlayer)
    , m_captured(false)
    , m_firstOwner(firstPlayer)
    , m_first(0)
    , m_second(0)
    , m_emptyCaptured(0)
  {
  }
  
  Owner StepQueue::firstOwner() const
  {
    return m_firstOwner;
  }
  
  QPoint StepQueue::lastPoint() const
  {
    return m_points.empty() ? QPoint{-1, -1} : m_points.back();
  }
  
  void  StepQueue::clear()
  {
    m_first = m_second = m_emptyCaptured = 0;
    m_owner  = m_firstOwner;
    m_captured  = false;
    m_firstPoints.clear();
    m_secondPoints.clear();
    m_points.clear();
  }

  void StepQueue::addPoint(const QPoint& point)
  {
    m_points.push_back(point);
    if (getCurrentOwner() == Owner::FIRST)
      m_firstPoints.push_back(point);
    else
      m_secondPoints.push_back(point);
  }

  void StepQueue::addCaptured()
  {
    m_captured = true;

    if (getCurrentOwner() == Owner::FIRST)
      ++m_first;
    else
      ++m_second;
  }
  
  void StepQueue::addEmptyCaptured()
  {
    ++m_emptyCaptured;
  }
  
  std::size_t StepQueue::emptyCapturedCount() const
  {
    return m_emptyCaptured;
  }

  Owner StepQueue::getCurrentOwner() const
  {
    return m_owner;
  }

  std::size_t StepQueue::getMarks(Owner owner) const
  {
    return owner == Owner::FIRST ? m_first : m_second;
  }

  const std::vector<QPoint>& StepQueue::getPoints(Owner owner) const
  {
    return owner == Owner::SECOND ? m_secondPoints : m_firstPoints;
  }
  
  const std::vector<QPoint>& StepQueue::getAllPoints() const
  {
    return m_points;
  }

  Owner StepQueue::other(Owner player)
  {
    if (player == Owner::NONE)
      qWarning() << "player == NONE";
    return player == Owner::FIRST ? Owner::SECOND : Owner::FIRST;
  }

  Owner StepQueue::nextStep()
  {
    m_captured = false;
    return (m_owner = other(m_owner));
  }


  ExtraStepQueue::ExtraStepQueue(Owner firstPlayer)
    : StepQueue(firstPlayer)
  {
  }
  
  Owner ExtraStepQueue::nextStep()
  {
    if (m_captured)
    {
      m_captured = false;
      return m_owner;
    }

    m_captured = false;

    return (m_owner = other(m_owner));
  }
}
