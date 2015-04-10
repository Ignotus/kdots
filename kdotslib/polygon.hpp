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
#include <vector>

#include <QScopedPointer>

#include "kdots_api.hpp"
#include "enums.hpp"

namespace std
{
  template<typename T>
  class shared_ptr;
}

class QPoint;

namespace KDots
{
  class PolygonPrivate;
  class KDOTS_EXPORT Polygon final
  {
    Q_DECLARE_PRIVATE(Polygon)
    Q_DISABLE_COPY(Polygon)
  public:
    Polygon(const std::vector<QPoint>& points = {});
    ~Polygon();
    
    int area() const;
    
    const std::vector<QPoint>& points() const;

    bool isFilled() const;

    void setFilled(bool filled);
    
    Owner owner() const;
    
    void setOwner(Owner own);
    
    bool contains(const QPoint& point) const;
  
  private:
    const QScopedPointer<PolygonPrivate> d_ptr;
  };
  
  typedef std::shared_ptr<Polygon> Polygon_ptr;
  typedef std::vector<Polygon_ptr> PolyList;
}