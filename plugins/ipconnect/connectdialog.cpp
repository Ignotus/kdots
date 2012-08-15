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
#include "connectdialog.hpp"
#include <QTcpServer>
#include <QDebug>
#include "ui_connectdialog.h"

namespace KDots
{
	namespace ipconnect
	{
		ConnectDialog::ConnectDialog (QTcpServer* server, int port, QWidget* parent)
			: QDialog (parent)
			, m_ui (new Ui::ConnectDialog)
			, m_server (server)
		{
			m_ui->setupUi (this);
			
			connect (server,
					SIGNAL (newConnection ()),
					this,
					SLOT (accept ()));
			
			if (m_server->listen (QHostAddress::Any, port))
				qDebug () << Q_FUNC_INFO << "Listening the port" << port;
			else
				qWarning () << Q_FUNC_INFO << "Couldn't listen the port " << port;
		}

	}
}

#include "connectdialog.moc"