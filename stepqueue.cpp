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

#include "stepqueue.hpp"

namespace KDots
{
  
	StepQueue::StepQueue (Owner firstPlayer)
		: m_owner (firstPlayer)
		, m_first (0)
		, m_second (0)
		, m_captured (false)
	{
	}

	void StepQueue::addPoint (const Point& point)
	{
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