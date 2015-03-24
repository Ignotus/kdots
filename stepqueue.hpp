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
#include <KDebug>
#include "point.hpp"
#include "constants.hpp"

namespace KDots
{
  class KDOTS_EXPORT StepQueue
  {
  public:
    StepQueue(Owner firstPlayer);
    virtual ~StepQueue() {}
    
    Owner firstOwner() const;
    Point lastPoint() const;
    
    void clear();

    void addPoint(const Point& point);
    void addCaptured();
    void addEmptyCaptured();
    
    std::size_t emtyCapturedCount() const;

    Owner getCurrentOwner() const;
    std::size_t getMarks(Owner owner) const;
    const std::vector<Point>& getPoints(Owner owner) const;
    const std::vector<Point>& getAllPoints() const;

    static Owner other(Owner player);

    Owner nextStep();
  
  protected:
    Owner m_owner;
    bool m_captured; 
    
  private:
    Owner m_firstOwner;
    std::vector<Point> m_firstPoints, m_secondPoints, m_points;
    std::size_t m_first, m_second, m_emptyCaptured;
  };

  class KDOTS_EXPORT ExtraStepQueue final : public StepQueue
  {
  public:
    ExtraStepQueue(Owner firstPlayer);

    Owner nextStep();
  };


}