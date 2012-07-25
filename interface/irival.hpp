/*
 * kdots
 * Copyright (C) 2011-2012 Minh Ngo <nlminhtl@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef KDOTS_IRIVAL_HPP
#define KDOTS_IRIVAL_HPP
#include <point.hpp>

class QObject;

namespace KDots
{
  class DotTable;
  
  class IRival
  {
  public:
    virtual ~IRival () {}
    
    virtual void configure (DotTable *table)
    {
    }
    
    virtual QObject* getQObject () = 0;
    
    virtual bool isAllow () const = 0;
    virtual void nextStep (const Point& point) = 0;
  };
}

#endif