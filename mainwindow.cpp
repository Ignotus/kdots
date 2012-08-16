/*
 * KDots
 * Copyright (c) 2011-2012 Minh Ngo <ignotusp@fedoraproject.org>
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
				statusBar (),
				SLOT (clearMessage ()));
		connect (table,
				SIGNAL (updateStatusBar (const QString&)),
				statusBar (),
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