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
		TableWidget *m_table;
		
		QAction *m_undoAction;
		
		void initMenu ();

	private slots:
		void on_actionNewGame_triggered ();
		void destroyGame ();
		void onPreferences ();
		void updateConfiguration (const QString& dialogName);
		void undo ();
	};
}

#endif
