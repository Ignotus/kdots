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
#include <KAction>
#include <KStandardAction>
#include <KActionCollection>
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
		, m_difficulty (new KgDifficulty (this))
	{
		m_ui->setupUi (this);
		
		m_difficulty->addStandardLevel (KgDifficultyLevel::Easy);
		m_difficulty->addStandardLevel (KgDifficultyLevel::Medium);
		m_difficulty->addStandardLevel (KgDifficultyLevel::Hard);
		
		connect (m_difficulty,
				SIGNAL (currentLevelChanged (const KgDifficultyLevel*)),
				this,
				SLOT (difficultyHandler (const KgDifficultyLevel*)));
		
		KgDifficultyGUI::init (this, m_difficulty);
		
		statusBar ()->show ();
		setCentralWidget (new QWidget (this));
		initMenu ();
		setupGUI (Default, "kdotsui.rc");
	}
	
	MainWindow::~MainWindow ()
	{
		m_rival.reset ();
	}
	
	void MainWindow::difficultyHandler (const KgDifficultyLevel *level)
	{
		int diff;
		switch (level->standardLevel ())
		{
		case KgDifficultyLevel::Easy:
			diff = 1;
		case KgDifficultyLevel::Medium:
			diff = 2;
		default:
			diff = 3;
		}
		
		if (m_rival)
			m_rival->setDifficulty (diff);
	}
	
	void MainWindow::initMenu ()
	{
		KStandardAction::preferences (this, SLOT (onPreferences ()), actionCollection ());
		
		KAction *newAction = new KAction (KIcon ("file_new"), i18n ("&New game"), this);
		newAction->setShortcut (Qt::CTRL + Qt::Key_N);
		
		connect (newAction,
				SIGNAL (triggered (bool)),
				this,
				SLOT (onNewGame ()));
		
		actionCollection ()->addAction ("NewGame", newAction);
		
		KAction *endAction = actionCollection ()->addAction ("EndGame", this, SLOT (endGame ()));
		endAction->setIcon (KIcon ("window-close"));
		endAction->setText (i18n ("&End game"));
		endAction->setShortcut (Qt::CTRL + Qt::Key_E);
		endAction->setEnabled (false);
		
		KAction *undoAction = actionCollection ()->addAction ("UndoGame", this, SLOT (undo ()));
		undoAction->setIcon (KIcon ("undo"));
		undoAction->setText (i18n ("&Undo"));
		undoAction->setEnabled (false);
		undoAction->setShortcut (Qt::CTRL + Qt::Key_Z);
		
		actionCollection ()->addAction ("UndoGame", undoAction);
		
		connect (this,
				SIGNAL (endActionEnable (bool)),
				endAction,
				SLOT (setEnabled (bool)));
		
		connect (this,
				SIGNAL (undoActionEnable (bool)),
				undoAction,
				SLOT (setEnabled (bool)));
	}
	
	void MainWindow::endGame ()
	{
		m_table->deleteLater ();
		m_table = NULL;
		m_rival.reset ();
		
		emit endActionEnable (false);
		
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

	void MainWindow::onNewGame ()
	{
		NewGameDialog dialog;
		if (dialog.exec () != QDialog::Accepted)
			return;
		
		m_rival = dialog.rival ();
		m_rival->setStatusBar (statusBar ());
		
		difficultyHandler (m_difficulty->currentLevel ());
		
		emit undoActionEnable (m_rival->canUndo ());
		
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
		
		emit endActionEnable (true);
	}
	
	void MainWindow::destroyGame ()
	{
		m_destroyTable = true;
		emit undoActionEnable (false);
	}
}

#include "include/mainwindow.moc"