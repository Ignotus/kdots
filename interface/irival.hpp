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
#ifndef KDOTS_IRIVAL_HPP
#define KDOTS_IRIVAL_HPP
#include <QObject>
#include <include/newgamewidget.hpp>

class QWidget;

namespace KDots
{
	class DotTable;
	class IConfigurationWidget;
	class Point;
	
	class IRival : public QObject
	{
		Q_OBJECT
	public:
		IRival (QObject *parent = 0)
			: QObject (parent)
		{
		}
		
		virtual ~IRival ()
		{
		}

		virtual IConfigurationWidget* configureWidget ()
		{
			return NULL;
		}
		
		virtual bool isAllow () const = 0;
		
		virtual GameConfig getGameConfig ()
		{
			return GameConfig ();
		}
		
	public slots:
		virtual void nextStep (const Point& point)
		{
			Q_UNUSED (point);
		}
		
		virtual void setDotTable (DotTable *table)
		{
			Q_UNUSED (table);
		}
		
	signals:
		void createDotTable (const GameConfig& config);
	};
}

#endif