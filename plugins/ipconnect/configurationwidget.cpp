/*
 * KDots
 * Copyright (c) 2011-2012 Minh Ngo <nlminhtl@gmail.com>
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
#include "configurationwidget.hpp"
#include "ui_configurationwidget.h"

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

#include "configurationwidget.moc"