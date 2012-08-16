/*
 * KDots
 * Copyright (c) 2011-2012 Minh Ngo <ignotusp@fedoraproject.org>
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
#ifndef KDOTS_POINT_HPP
#define KDOTS_POINT_HPP
#include <QMetaType>
#include "constants.hpp"

namespace KDots
{
	class KDOTS_EXPORT Point
	{
		int m_x, m_y;
	public:
		inline Point ()
			: m_x (-1)
			, m_y (-1)
		{}
		
		inline Point (int x, int y)
			: m_x (x)
			, m_y (y)
		{}

		inline int x () const
		{
			return m_x;
		}

		inline int y () const
		{
			return m_y;
		}

		inline void setX (int x)
		{
			m_x = x;
		}

		inline void setY (int y)
		{
			m_y = y;
		}

		inline bool operator== (const Point& a) const
		{
			return m_x == a.m_x && m_y == a.m_y;
		}

		inline bool operator!= (const Point& a) const
		{
			return ! (*this == a);
		}

		inline bool empty () const
		{
			return m_x == -1 || m_y == -1;
		}
		
		friend QDataStream& operator<< (QDataStream& out, const KDots::Point& obj);
		friend QDataStream& operator>> (QDataStream& in, KDots::Point& obj);
	};
	
	QDataStream& operator<< (QDataStream& out, const KDots::Point& obj);

	QDataStream& operator>> (QDataStream& in, KDots::Point& obj);
}

Q_DECLARE_METATYPE (KDots::Point);



#endif