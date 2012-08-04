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
#ifndef KDOTS_ICONFIGURATIONWIDGET_HPP
#define KDOTS_ICONFIGURATIONWIDGET_HPP
#include <QWidget>
#include <QDebug>

namespace KDots
{
	struct GameConfig;
	class IConfigurationWidget : public QWidget
	{
		Q_OBJECT
	public:
		IConfigurationWidget (QWidget *parent = 0)
			: QWidget (parent)
		{
		}
	
	//public slots:
	//	virtual void ready ()
	//	{
	//	}
		
	signals:
		void needCreateTable (bool);
	};
}

#endif