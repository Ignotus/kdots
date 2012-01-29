/*
 * kdots
 * Copyright (C) 2011 Minh Ngo <nlminhtl@gmail.com>
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

#include "stepqueue.h"

StepQueue::StepQueue (GameMode mode, Owner owner)
: gameMode (mode)
, currentOwner (owner)
, firstPlayerCaptured (0)
, secondPlayerCaptured (0)
{
}

Owner StepQueue::nextStep (bool captured)
{
	if (!captured || gameMode != ExtraTurnGameMode)
		currentOwner = otherPlayer (currentOwner);
	
	return currentOwner;
}

Owner StepQueue::getCurrentOwner () const
{
	return currentOwner;
}

void StepQueue::addPoint (const IntPoint& point)
{
	if (getCurrentOwner () == FirstOwner)
		firstPlayerPoints.push_back (point);
	else
		secondPlayerPoints.push_back (point);
}

std::list<IntPoint> StepQueue::getOtherPointList () const
{
	return getCurrentOwner () == FirstOwner
			? secondPlayerPoints
			: firstPlayerPoints;
}

void StepQueue::addCaptured (int point)
{
	if (getCurrentOwner () == FirstOwner)
		firstPlayerCaptured += point;
	else
		secondPlayerCaptured += point;
}