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
#include "rival.hpp"
#include <QMessageBox>
#include <include/stepqueue.hpp>
#include "connectdialog.hpp"

namespace KDots
{
	namespace ipconnect
	{
		Rival::Rival (QObject *parent)
			: IRival (parent)
			, m_socket (NULL)
			, m_server (NULL)
			, m_me (NONE)
		{
		}
		
		Rival::~Rival ()
		{
			if (m_socket)
				m_socket->disconnectFromHost ();
		}
		
		namespace
		{
			Owner itemToOwner (int index)
			{
				if (!index)
					return Owner::FIRST;
				else
					return Owner::SECOND;
			}
		}
		
		void Rival::onDisconnected ()
		{
			QMessageBox::warning (0,
					tr ("The socket has been disconnected"),
					tr ("The socket has been disconnected"));
		}
		
		GameConfig Rival::getGameConfig ()
		{
			ClientConfig config;
			
			if (!m_configWidget->clientConfig (config))
			{
				qWarning () << Q_FUNC_INFO;
				return GameConfig ();
			}
			
			//Joining Game
			m_socket = new QTcpSocket (this);
			connect (m_socket,
					SIGNAL (disconnected ()),
					this,
					SLOT (onDisconnected ()));
			
			m_socket->connectToHost (config.m_host, config.m_port);

			qDebug () << Q_FUNC_INFO << "Connecting to the server...";
			if (m_socket->waitForConnected (5000))
			{
				qDebug () << Q_FUNC_INFO << "Connected";
				
				if (m_socket->waitForReadyRead ())
				{
					qDebug () << Q_FUNC_INFO << "Reading Table config";
					const QByteArray& data = m_socket->readAll();
					qDebug () << Q_FUNC_INFO << "Data size" << data.size ();
					QDataStream in (&const_cast<QByteArray&> (data), QIODevice::ReadOnly);
					QVariant variantData;
					
					quint32 me;
					in >> variantData >> me;
					m_me = static_cast<Owner> (me);
					
					if (!variantData.canConvert<GameConfig> ())
					{
						qWarning () << Q_FUNC_INFO << "Cannot convert to GameConfig: "
								<< variantData.typeName ();
					}
					
					const GameConfig& config = variantData.value<GameConfig> ();
					if (!config.isInititialized ())
					{
						qWarning () << Q_FUNC_INFO << "Table config is invalid";
						return GameConfig ();
					}
					else
					{
						connect (m_socket,
								SIGNAL (readyRead ()),
								this,
								SLOT (onReadyRead ()));
						
						QMessageBox::information (0,
								tr ("Connected"),
								tr ("Good luck have fun"));
						return config;
					}
				}
			}
			else
			{
				QMessageBox::warning (0,
						tr ("Warning!"),
						tr ("Cannot connect to the server: ") + m_socket->errorString ());
				
				emit needDestroy ();
			}
			
			return GameConfig ();
		}
		
		void Rival::setDotTable (DotTable *table) //Is called after configureWidget
		{
			m_table = table;
			
			ServerConfig config;
			if (!m_configWidget->serverConfig (config))
			{
				// Client
				return;
			}
			
			m_me = itemToOwner (config.user);
			
			//Create server
			
			qDebug () << Q_FUNC_INFO;
			m_server = new QTcpServer (this);

			connect (m_server,
					SIGNAL (newConnection ()),
					this,
					SLOT (onNewConnectionHandle ()));
			
			ConnectDialog dialog (m_server, config.m_port);
			if (dialog.exec () != QDialog::Accepted)
			{
				m_server->close ();
				emit needDestroy ();
			}
		}
		
		IConfigurationWidget* Rival::configureWidget ()
		{
			m_configWidget.reset (new ConfigurationWidget);
			ConfigurationWidget *w = m_configWidget.get ();

			return w;
		}

		bool Rival::isAllow () const
		{
			return m_table->stepQueue ()->getCurrentOwner () == m_me;
		}

		void Rival::nextStep (const Point& point)
		{
			if (!m_socket)
				return;
			
			qDebug () << Q_FUNC_INFO << "Sending point";
			QByteArray array;
			QDataStream out (&array, QIODevice::WriteOnly);
			
			out << QVariant::fromValue<Point> (point);
			qDebug () << Q_FUNC_INFO << m_socket->write (array);
		}

		void Rival::onNewConnectionHandle ()
		{
			qDebug () << Q_FUNC_INFO;
			m_socket = m_server->nextPendingConnection ();
			if (!m_socket)
				return;
			
			QByteArray gameData;
			QDataStream out (&gameData, QIODevice::WriteOnly);
			out << QVariant::fromValue<GameConfig> (m_table->gameConfig ())
					<< static_cast<quint32> (StepQueue::other (m_me));
			m_socket->write (gameData);
			qDebug () << Q_FUNC_INFO << "Game config sent";
			connect (m_socket,
					SIGNAL (readyRead ()),
					this,
					SLOT (onReadyRead ()));
		}

		void Rival::onReadyRead ()
		{
			const QByteArray& array = m_socket->readAll ();
			QDataStream in (&const_cast<QByteArray&> (array), QIODevice::ReadOnly);
			QVariant var;
			in >> var;
			const Point& point  = var.value<Point> ();
			m_table->pushPoint (point);
		}
	}
}