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
#ifndef KDOTS_MAINWINDOW_HPP
#define KDOTS_MAINWINDOW_HPP
#include <memory>
#include <KXmlGuiWindow>
#include "constants.hpp"

namespace Ui
{
	class MainWindow;
}

namespace KDots
{
	class TableWidget;
	class IRival;

	class KDOTS_EXPORT MainWindow : public KXmlGuiWindow
	{
		Q_OBJECT
	public:
		MainWindow (QWidget *parent = 0);
	private:
		Ui::MainWindow *m_ui;
		std::shared_ptr<IRival> m_rival;
		bool m_destroyTable;
		
		void initMenu ();

	private slots:
		void on_actionNewGame_triggered ();
		void destroyGame ();
	};
}

#endif
