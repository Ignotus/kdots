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
#ifndef KDOTS_DOTTABLE_HPP
#define KDOTS_DOTTABLE_HPP
#include <memory>
#include <QObject>
#include "graph.hpp"
#include "gameconfig.hpp"
#include "polygon.hpp"

namespace KDots
{
	class StepQueue;
	class DotTable : public QObject
	{
		Q_OBJECT

		Graph m_graph;
		std::shared_ptr<StepQueue> m_steps;
		GameConfig m_config;
	public:
		DotTable (const GameConfig& config, QObject *parent = 0);

		GameConfig gameConfig () const;

		void pushPoint (const Point& point);

		inline Graph graph () const
		{
			return m_graph;
		}

		inline std::shared_ptr<StepQueue> stepQueue()
		{
			return m_steps;
		}
	signals:
		void nextPlayer(const Point& lastPoint);
	private:
		void drawPolygon(PolyList polygons);
	};
}

#endif
