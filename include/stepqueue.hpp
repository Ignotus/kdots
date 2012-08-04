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
#ifndef KDOTS_STEPQUEUE_HPP
#define KDOTS_STEPQUEUE_HPP
#include <list>
#include "point.hpp"
#include "constants.hpp"

namespace KDots
{

	class StepQueue
	{
		std::list<Point> m_firstPoints, m_secondPoints;
		int m_first, m_second;
	protected:
		Owner m_owner;
		bool m_captured;
	public:
		StepQueue (Owner firstPlayer);

		void addPoint (const Point &point);
		void addCaptured ();

		inline Owner getCurrentOwner () const
		{
			return m_owner;
		}

		inline int getMarks (Owner owner) const
		{
			return owner == FIRST ? m_first : m_second;
		}

		inline std::list<Point> getPoints (Owner owner) const
		{
			return getCurrentOwner() == FIRST ? m_secondPoints : m_firstPoints;
		}

		inline static Owner other (Owner player)
		{
			return player == FIRST ? SECOND : FIRST;
		}

		virtual Owner nextStep ()
		{
			m_captured = false;
			return (m_owner = other (m_owner));
		}
	};

	class ExtraStepQueue : public StepQueue
	{
	public:
		ExtraStepQueue(Owner firstPlayer);

		Owner
		nextStep()
		{
			if(m_captured)
				return m_owner;

			m_captured = false;

			return (m_owner = other(m_owner));
		}
	};


}

#endif