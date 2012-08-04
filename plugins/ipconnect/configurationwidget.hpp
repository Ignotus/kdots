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
#ifndef KDOTS_PLUGINS_IPCONNECT_CONFIGURATIONWIDGET_H
#define KDOTS_PLUGINS_IPCONNECT_CONFIGURATIONWIDGET_H
#include <interface/iconfigurationwidget.hpp>

namespace Ui
{
	class ConfigurationWidget;
}


namespace KDots
{
	namespace ipconnect
	{
		struct ServerConfig
		{
			int m_port;
			int user;
		};
		
		struct ClientConfig
		{
			QString m_host;
			int m_port;
		};
		
		class ConfigurationWidget : public IConfigurationWidget
		{
			Q_OBJECT

			Ui::ConfigurationWidget *m_ui;
		public:
			ConfigurationWidget (QWidget *parent = 0);
			bool clientConfig (ClientConfig& config);
			bool serverConfig (ServerConfig& config);
			
		private slots:
			void onTabChanged (int index);
		};
	}
}

#endif
