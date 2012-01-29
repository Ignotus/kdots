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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
	class MainWindow;
}

class TableWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow (QWidget *parent = 0);
	virtual ~MainWindow ();

private:
	Ui::MainWindow *ui;
	TableWidget *table;
	
private slots:
	void on_actionNewGame_triggered ();
};

#endif // MAINWINDOW_H
