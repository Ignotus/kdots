/*
 * kdots
 * Copyright (C) 2011 Minh Ngo <nlminhtl@gmail.com>
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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newgamedialog.h"
#include "tablewidget.h"

MainWindow::MainWindow (QWidget *parent)
: QMainWindow (parent)
, ui (new Ui::MainWindow)
, table (NULL)
{
	ui->setupUi (this);
}

MainWindow::~MainWindow ()
{
	delete ui;
}

void MainWindow::on_actionNewGame_triggered ()
{
	NewGameDialog dialog;
	if (dialog.exec () == QDialog::Rejected)
		return;
	
	table = new TableWidget (dialog.getHeight (),
			dialog.getWidth (),
			dialog.getGameMode (),
			dialog.getFirstMoving (),
			this);
	connect (table,
			SIGNAL (updateStatusBar (const QString&)),
			ui->statusBar,
			SLOT (clearMessage ()));
	connect (table,
			SIGNAL (updateStatusBar (const QString&)),
			ui->statusBar,
			SLOT (showMessage (const QString&)));

	setCentralWidget (table);
	table->show ();
}

