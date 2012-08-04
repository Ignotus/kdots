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

#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QDir>
#include <QDebug>
#include "interface/iplugin.hpp"
#include "newgamedialog.hpp"
#include "tablewidget.hpp"
#include "plugincontainer.hpp"

namespace KDots
{
	MainWindow::MainWindow (QWidget *parent)
		: QMainWindow (parent)
		, m_ui (new Ui::MainWindow)
	{
		m_ui->setupUi (this);
	}

	void MainWindow::on_actionNewGame_triggered ()
	{
		qDebug () << Q_FUNC_INFO;	
		NewGameDialog dialog;
		qDebug () << Q_FUNC_INFO;	
		if (dialog.exec () != QDialog::Accepted)
			return;

		m_rival = dialog.rival ();

		const GameConfig& config = dialog.gameConfig ();
		
		if (!config.isInititialized ())
			return;

		TableWidget *table = new TableWidget (config, m_rival, this);

		connect (table,
		         SIGNAL (updateStatusBar (const QString&)),
		         m_ui->statusBar,
		         SLOT (clearMessage ()));
		connect (table,
		         SIGNAL (updateStatusBar (const QString&)),
		         m_ui->statusBar,
		         SLOT (showMessage (const QString&)));

		setCentralWidget (table);
		table->show ();

		//connect (&dialog,
		//		SIGNAL (gameConfig (const GameConfig&)),
		//		this,
		//		SLOT (onGameConfig (const GameConfig&)));
	}

}