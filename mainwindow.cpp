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
#include "mainwindow.hpp"
#include <QDir>
#include <KMenuBar>
#include <KStatusBar>
#include <KConfigDialog>
#include <KToolBar>
#include <interface/iplugin.hpp>
#include <interface/irival.hpp>
#include "ui_mainwindow.h"
#include "ui_boardconfigwidget.h"
#include "newgamedialog.hpp"
#include "tablewidget.hpp"
#include "plugincontainer.hpp"
#include "kdots.h"

namespace KDots
{
	MainWindow::MainWindow (QWidget *parent)
		: KXmlGuiWindow (parent)
		, m_ui (new Ui::MainWindow)
		, m_destroyTable (false)
		, m_table (NULL)
		, m_undoAction (NULL)
		, m_endAction (NULL)
	{
		m_ui->setupUi (this);
		
		statusBar ()->show ();
		setCentralWidget (new QWidget (this));
		setupGUI (Create, "kdotsui.rc");
		initMenu ();
	}
	
	void MainWindow::initMenu ()
	{
		KMenuBar *currentBar = menuBar ();
		
		QMenu *settingMenu = new QMenu (i18n ("&Settings"));
		QAction *settingAction = currentBar
				->insertMenu (currentBar->actionAt ({1, 1}), settingMenu);
		
		settingMenu->addAction (KIcon ("configure"), i18n ("&Preferences..."),
				this, SLOT (onPreferences ()));
		
		QMenu *fileMenu = new QMenu (i18n ("&Game"));
		fileMenu->addAction (KIcon ("file_new"), i18n ("&New game"), this,
				SLOT (on_actionNewGame_triggered ()));
		
		currentBar->insertMenu (settingAction, fileMenu);
		menuBar ()->show ();
		
		KToolBar *currentTools = toolBar ();
		currentTools->addAction (KIcon ("file_new"), i18n ("&New game"), this,
				SLOT (on_actionNewGame_triggered ()));
		m_endAction = currentTools->addAction (KIcon ("window-close"),
				i18n ("&End game"), this, SLOT (endGame ()));
		m_endAction->setEnabled (false);
		
		m_undoAction = currentTools->addAction (KIcon ("undo"), i18n ("Undo"), this, SLOT (undo ()));
		m_undoAction->setEnabled (false);
		currentTools->show ();
	}
	
	void MainWindow::endGame ()
	{
		delete m_table;
		m_table = NULL;
		m_rival.reset ();
		m_endAction->setEnabled (true);
		statusBar ()->clearMessage ();
	}
	
	void MainWindow::undo ()
	{
		if (m_table && m_rival && m_rival->canUndo ())
			m_table->undo ();
	}
	
	void MainWindow::onPreferences ()
	{
		KConfigDialog dialog (this, i18n ("Preferences"), Settings::self ());
		
		QWidget *board = new QWidget;
		
		Ui::BoardConfigWidget *boardUi = new Ui::BoardConfigWidget;
		boardUi->setupUi (board);
			
		dialog.addPage (board, i18n ("Board"), QLatin1String ("games-config-options"));
		
		if (m_table)
		{
			connect (&dialog,
					SIGNAL (accepted ()),
					m_table,
					SLOT (update ()));
		}
		
		dialog.exec ();
	}

	void MainWindow::on_actionNewGame_triggered ()
	{
		NewGameDialog dialog;
		if (dialog.exec () != QDialog::Accepted)
			return;

		m_rival = dialog.rival ();
		
		m_undoAction->setEnabled (m_rival->canUndo ());
		
		connect (m_rival.get (),
				SIGNAL (needDestroy ()),
				this,
				SLOT (destroyGame ()));

		const GameConfig& config = dialog.gameConfig ();
		
		if (!config.isInititialized ())
			return;

		m_table = new TableWidget (config, m_rival, this);
		
		if (m_destroyTable)
		{
			endGame ();
			return;
		}

		connect (m_table,
				SIGNAL (updateStatusBar (const QString&)),
				statusBar (),
				SLOT (clearMessage ()));
		connect (m_table,
				SIGNAL (updateStatusBar (const QString&)),
				statusBar (),
				SLOT (showMessage (const QString&)));

		setCentralWidget (m_table);
		m_table->show ();
		m_endAction->setEnabled (true);
	}
	
	void MainWindow::destroyGame ()
	{
		m_destroyTable = true;
		m_undoAction->setEnabled (false);
	}
}

#include "include/mainwindow.moc"