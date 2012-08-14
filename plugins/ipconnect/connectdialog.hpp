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
#ifndef KDOTS_PLUGINS_IPCONNECT_CONNECTDIALOG_HPP
#define KDOTS_PLUGINS_IPCONNECT_CONNECTDIALOG_HPP
#include <QDialog>

namespace Ui
{
	class ConnectDialog;
}

class QTcpServer;

namespace KDots
{
	namespace ipconnect
	{
		class ConnectDialog : public QDialog
		{
			Q_OBJECT
			
			Ui::ConnectDialog *m_ui;
			QTcpServer *m_server;
		public:
			ConnectDialog (QTcpServer *server, int port, QWidget *parent = 0);
		};
	}
}

#endif
