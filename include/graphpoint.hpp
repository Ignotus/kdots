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
#ifndef KDOTS_GRAPHPOINT_HPP
#define KDOTS_GRAPHPOINT_HPP
#include "edgelist.hpp"
#include "constants.hpp"

namespace KDots
{
  
  class GraphPoint
  {
  public:
    typedef EdgeList<8> GraphEdges;
    
  private:
    bool m_captured;
    Owner m_owner;
    GraphEdges m_edges;
    
  public:
    inline
    GraphPoint (Owner owner = NONE)
      : m_captured (false)
      , m_owner (owner)
    {
    }
    
    inline bool
    isCaptuted () const
    {
      return m_captured;
    }
    
    inline void
    capture ()
    {
      m_captured = true;
    }
    
    inline void
    setOwner (Owner owner)
    {
      m_owner = owner;
    }
    
    inline int
    owner () const
    {
      return m_owner;
    }
    
    GraphEdges& edges ()
    {
      return m_edges;
    }
    
    const GraphEdges& edges () const
    {
      return m_edges;
    }
  };
  
}

#endif
