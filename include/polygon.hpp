/*
 * KDots
 * Copyright (c) 2011-2012 Minh Ngo <nlminhtl@gmail.com>
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
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef KDOTS_POLYGON_HPP
#define KDOTS_POLYGON_HPP
#include <list>
#include <vector>
#include <memory>
#include "point.hpp"
#include "constants.hpp"

namespace KDots
{
	class KDOTS_EXPORT Polygon : public std::list<Point>
	{
		bool m_filled;
		Owner m_owner;
		float m_area;
	public:
		Polygon ()
			: std::list<Point> ()
			, m_filled (false)
		{
		}

		Polygon (const std::list<Point>& a)
			: std::list<Point> (a)
			, m_filled (false)
		{
		}
		
		void setArea (float area)
		{
			m_area = area;
		}
		
		float area () const
		{
			return m_area;
		}

		bool isFilled () const
		{
			return m_filled;
		}

		void setFilled (bool filled)
		{
			m_filled = filled;
		}
		
		Owner owner () const
		{
			return m_owner;
		}
		
		void setOwner (Owner own)
		{
			m_owner = own;
		}
	};
	
	typedef std::shared_ptr<Polygon> Polygon_ptr;
	typedef std::vector<Polygon_ptr> PolyList;
}

#endif