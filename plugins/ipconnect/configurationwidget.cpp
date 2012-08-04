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
#include "configurationwidget.hpp"
#include "ui_configurationwidget.h"
#include <QDebug>

namespace KDots
{
	namespace ipconnect
	{
		namespace
		{
			enum Tabs
			{
				CreateGameTab = 0,
				JoinGameTab = 1
			};
		}
		
		ConfigurationWidget::ConfigurationWidget (QWidget *parent)
			: IConfigurationWidget (parent)
			, m_ui (new Ui::ConfigurationWidget)
		{
			m_ui->setupUi (this);
			
			connect (m_ui->tabWidget,
					SIGNAL (currentChanged (int)),
					this,
					SLOT (onTabChanged (int)));
		}
		
		void ConfigurationWidget::onTabChanged (int index)
		{
			emit needCreateTable (index == CreateGameTab);
		}
		
		bool ConfigurationWidget::clientConfig (ClientConfig& config)
		{
			if (m_ui->tabWidget->currentIndex () == CreateGameTab)
				return false;
			else
			{
				config.m_host = m_ui->host->text ();
				config.m_port = m_ui->connectionPort->value ();
				return true;
			}
		}
		
		bool ConfigurationWidget::serverConfig (ServerConfig& config)
		{
			if (m_ui->tabWidget->currentIndex () == JoinGameTab)
				return false;
			else
			{
				config.m_port = m_ui->listenPort->value ();
				config.user = m_ui->user->currentIndex ();
				return true;
			}
		}

	}
}