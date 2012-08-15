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
#include "newgamedialog.hpp"
#include <QDebug>
#include "ui_newgamedialog.h"
#include "pluginmanagerwidget.hpp"
#include "newgamewidget.hpp"
#include "plugincontainer.hpp"
#include <interface/irival.hpp>
#include <interface/iplugin.hpp>
#include <interface/iconfigurationwidget.hpp>

namespace KDots
{
	NewGameDialog::NewGameDialog (QWidget *parent)
		: QDialog (parent)
		, m_ui (new Ui::NewGameDialog)
		, m_game (NULL)
		, m_pluginManager (new PluginManagerWidget (this))
		, m_configWidget (NULL)
	{
		m_ui->setupUi (this);
			
		m_ui->Grid->addWidget (m_pluginManager, 0, 0, Qt::AlignCenter);
		
		connect (m_ui->NextButton,
				SIGNAL (clicked (bool)),
				this,
				SLOT (pluginWidget ()));
	}
	
	NewGameDialog::~NewGameDialog ()
	{
		if (m_configWidget)
			m_configWidget->setParent(0);
	}
	
	std::shared_ptr<IRival> NewGameDialog::rival () const
	{
		return m_rival;
	}
	
	GameConfig NewGameDialog::gameConfig () const
	{
		if (m_game)
			return m_game->getGameConfig ();
		
		return m_rival->getGameConfig ();
	}
	
	void NewGameDialog::pluginWidget ()
	{
		m_ui->NextButton->disconnect (this, SLOT (pluginWidget ()));
		
		if (!m_pluginManager)
		{
			qWarning () << Q_FUNC_INFO << "Cannot cast to PluginManagerWidget";
			return;
		}
		
		const QString& pluginName = m_pluginManager->pluginName ();
		
		IPlugin *pluginInstance = PluginContainer::instance ().plugin (pluginName);
		if (!pluginInstance)
		{
			qDebug () << Q_FUNC_INFO << "Plugin instance not exists";
			return;
		}
		
		m_rival.reset (pluginInstance->createRival ());
		
		m_pluginManager->hide ();
		
		m_configWidget = m_rival->configureWidget ();
		
		if (!m_configWidget)
		{
			gameWidget ();
			return;
		}
		
		m_ui->Grid->addWidget (m_configWidget , 0, 0, Qt::AlignCenter);
		
		connect (m_configWidget,
				SIGNAL (needCreateTable (bool)),
				this,
				SLOT (onNeedCreateTable (bool)));
		
		connect (m_ui->NextButton,
				SIGNAL (clicked (bool)),
				this,
 				SLOT (gameWidget ()));
	}
	
	void NewGameDialog::onNeedCreateTable (bool val)
	{
		if (val)
		{
			m_ui->NextButton->setEnabled (true);
			m_ui->OKButton->setEnabled (false);
			connect (m_ui->NextButton,
					SIGNAL (clicked (bool)),
					this,
					SLOT (gameWidget ()));
		}
		else
		{
			m_ui->NextButton->setEnabled (false);
			m_ui->OKButton->setEnabled (true);
			m_ui->NextButton->disconnect (this, SLOT (gameWidget ()));
		}
	}
	
	void NewGameDialog::gameWidget ()
	{
		if (m_configWidget)
			m_configWidget->hide ();
		
		m_ui->NextButton->setEnabled (false);
		m_ui->OKButton->setEnabled (true);
		m_game = new NewGameWidget (this);
		m_ui->Grid->addWidget (m_game, 0, 0, Qt::AlignCenter);
	}
	
}

#include "include/newgamedialog.moc"
