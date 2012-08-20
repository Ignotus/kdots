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
#ifndef KDOTS_GRAPHPOINT_HPP
#define KDOTS_GRAPHPOINT_HPP
#include "edgelist.hpp"
#include "constants.hpp"

namespace KDots
{
	class KDOTS_EXPORT GraphPoint
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

		inline bool isCaptured () const
		{
			return m_captured;
		}

		inline void capture ()
		{
			m_captured = true;
		}

		inline void setOwner (Owner owner)
		{
			m_owner = owner;
		}

		inline int owner () const
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
