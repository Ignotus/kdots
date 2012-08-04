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
#ifndef KDOTS_NEWGAMEDIALOG_HPP
#define KDOTS_NEWGAMEDIALOG_HPP
#include <memory>
#include <QDialog>
#include <include/newgamewidget.hpp>

namespace Ui
{
	class NewGameDialog;
}

namespace KDots
{
	class IRival;
	class IConfigurationWidget;
	class PluginManagerWidget;
	
	class NewGameDialog : public QDialog
	{
		Q_OBJECT
		
		Ui::NewGameDialog *m_ui;
		NewGameWidget *m_game;
		PluginManagerWidget *m_pluginManager;
		IConfigurationWidget *m_configWidget;
	
		mutable std::shared_ptr<IRival> m_rival;
	public:
		NewGameDialog (QWidget *parent = 0);
		
		std::shared_ptr<IRival> rival () const;
		
		GameConfig gameConfig () const;
	private slots:
		void pluginWidget ();
		void gameWidget ();
		void onNeedCreateTable (bool);
	//signals:
	//	void createTable (const GameConfig& config);
	};
}

#endif