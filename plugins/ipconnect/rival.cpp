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
#include <stepqueue.hpp>

namespace KDots
{
	namespace ipconnect
	{
		Rival::Rival (QObject *parent)
			: IRival (parent)
			, m_socket (NULL)
		{
		}
		
		Rival::~Rival ()
		{
			if (m_socket)
				m_socket->disconnectFromHost ();
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

			m_socket->connectToHost (config.m_host, config.m_port);

			qDebug () << Q_FUNC_INFO << "Connecting to the server...";
			if (m_socket->waitForConnected ())
			{
				qDebug () << Q_FUNC_INFO << "Connected";
				
				if (m_socket->waitForReadyRead ())
				{
					qDebug () << Q_FUNC_INFO << "Reading Table config";
					const QByteArray& data = m_socket->readAll();
					qDebug () << Q_FUNC_INFO << "Data size" << data.size ();
					QDataStream in (&const_cast<QByteArray&> (data), QIODevice::ReadOnly);
					QVariant variantData;
					in >> variantData;
					if (!variantData.canConvert<GameConfig> ())
					{
						qWarning () << Q_FUNC_INFO << "Cannot convert to GameConfig: " << variantData.typeName ();
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
						return config;
					}
				}
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
			
			//Create server
			
			qDebug () << Q_FUNC_INFO;
			m_server = new QTcpServer (this);

			connect (m_server,
					SIGNAL (newConnection ()),
					this,
					SLOT (onNewConnectionHandle ()));
			

			if (m_server->listen (QHostAddress::Any, config.m_port))
				qDebug () << Q_FUNC_INFO << "Listening the port" << config.m_port;
			else
				qWarning () << Q_FUNC_INFO << "Couldn't listen the port " << config.m_port;
		}
		
		IConfigurationWidget* Rival::configureWidget ()
		{
			m_configWidget.reset (new ConfigurationWidget);
			ConfigurationWidget *w = m_configWidget.get ();

			return w;
		}

		bool Rival::isAllow () const
		{
			return true;
		}

		void Rival::nextStep (const Point& point)
		{
			QByteArray array;
			QDataStream out (&array, QIODevice::WriteOnly);
			
			out << QVariant::fromValue<Point> (point);
			
			m_socket->write (array);
		}

		void Rival::onNewConnectionHandle ()
		{
			qDebug () << Q_FUNC_INFO;
			m_socket = m_server->nextPendingConnection ();
		
			QByteArray gameData;
			QDataStream out (&gameData, QIODevice::WriteOnly);
			out << QVariant::fromValue<GameConfig> (m_table->gameConfig ());
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