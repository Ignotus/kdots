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
#include <KMenuBar>
#include <KStatusBar>
#include <interface/iplugin.hpp>
#include <interface/irival.hpp>
#include "newgamedialog.hpp"
#include "tablewidget.hpp"
#include "plugincontainer.hpp"

namespace KDots
{
	MainWindow::MainWindow (QWidget *parent)
		: KXmlGuiWindow (parent)
		, m_ui (new Ui::MainWindow)
		, m_destroyTable (false)
	{
		m_ui->setupUi (this);
		
		statusBar ()->show ();
		setCentralWidget (new QWidget (this));
		setupGUI (Create);
		initMenu ();
	}
	
	void MainWindow::initMenu ()
	{
		KMenuBar *currentBar = menuBar ();
		QMenu *fileMenu = new QMenu (tr ("&File"));
		fileMenu->addAction (tr ("New game"), this, SLOT (on_actionNewGame_triggered ()));
		currentBar->insertMenu (currentBar->actionAt ({1, 1}), fileMenu);
		menuBar ()->show ();
	}

	void MainWindow::on_actionNewGame_triggered ()
	{
		NewGameDialog dialog;
		if (dialog.exec () != QDialog::Accepted)
			return;

		m_rival = dialog.rival ();
		
		connect (m_rival.get (),
				SIGNAL (needDestroy ()),
				this,
				SLOT (destroyGame ()));

		const GameConfig& config = dialog.gameConfig ();
		
		if (!config.isInititialized ())
			return;

		TableWidget *table = new TableWidget (config, m_rival, this);
		
		if (m_destroyTable)
		{
			delete table;
			return;
		}

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
	}
	
	void MainWindow::destroyGame ()
	{
		m_destroyTable = true;
	}

}

#include "include/mainwindow.moc"