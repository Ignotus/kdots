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
#ifndef KDOTS_GRAPH_HPP
#define KDOTS_GRAPH_HPP
#include <vector>
#include "graphpoint.hpp"

namespace KDots
{
	class KDOTS_EXPORT Graph
	{
		std::vector<std::vector<GraphPoint>> m_graph;
	public:
		Graph (int width, int height);

		inline int width () const
		{
			return m_graph.size ();
		}

		inline int height () const
		{
			return m_graph.front ().size ();
		}

		inline GraphPoint& operator[] (const Point& index)
		{
			return m_graph[index.x ()][index.y ()];
		}

		inline const GraphPoint& operator[] (const Point& index) const
		{
			return m_graph[index.x ()][index.y ()];
		}

		inline std::vector<GraphPoint>& operator[] (int index)
		{
			return m_graph[index];
		}

		inline const std::vector<GraphPoint>& operator[] (int index) const
		{
			return m_graph[index];
		}

		void addEdge (const KDots::Point& first, const KDots::Point& second);
		void removeEdge (const Point& first, const Point& second);
	};
}

#endif
