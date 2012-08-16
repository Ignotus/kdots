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
#include "stepqueue.hpp"

namespace KDots
{
  
	StepQueue::StepQueue (Owner firstPlayer)
		: m_first (0)
		, m_second (0)
		, m_owner (firstPlayer)
		, m_captured (false)
	{
	}

	void StepQueue::addPoint (const Point& point)
	{
		m_lastPoint = point;
		if (getCurrentOwner () == FIRST)
			m_firstPoints.push_back (point);
		else
			m_secondPoints.push_back (point);
	}

	void StepQueue::addCaptured ()
	{
		m_captured = true;

		if (getCurrentOwner () == FIRST)
			++m_first;
		else
			++m_second;
	}

	ExtraStepQueue::ExtraStepQueue (Owner firstPlayer)
		: StepQueue (firstPlayer)
	{
	}

}