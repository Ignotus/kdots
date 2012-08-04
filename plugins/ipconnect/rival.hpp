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
#ifndef KDOTS_PLUGINS_IPCONNECT_RIVAL_HPP
#define KDOTS_PLUGINS_IPCONNECT_RIVAL_HPP
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <interface/irival.hpp>
#include <include/dottable.hpp>
#include "configurationwidget.hpp"

namespace KDots
{
	namespace ipconnect
	{
		
		class Rival : public KDots::IRival
		{
			Q_OBJECT

			DotTable *m_table;
			QTcpSocket *m_socket;
			QTcpServer *m_server;
			
			Owner m_me;
			
			std::unique_ptr<ConfigurationWidget> m_configWidget;
		public:
			Rival (QObject *parent = 0);
			virtual ~Rival ();
			
			GameConfig getGameConfig ();
			
			IConfigurationWidget* configureWidget ();

			void setDotTable (DotTable *table);

			bool isAllow () const;

		public slots:
			void nextStep (const Point& point);
			
		private slots:
			void onNewConnectionHandle ();
			void onReadyRead ();
		};
	}
}
#endif
