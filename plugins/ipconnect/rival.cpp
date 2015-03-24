/*
 * KDots
 * Copyright (c) 2011, 2012, 2014, 2015 Minh Ngo <minh@fedoraproject.org>
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
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "rival.hpp"
#include <KMessageBox>
#include <KDebug>
#include <KLocalizedString>
#include <KgDifficulty>
#include <boardmodel.hpp>
#include <stepqueue.hpp>
#include <graph.hpp>
#include "connectdialog.hpp"
#include "configurationwidget.hpp"

namespace KDots
{
  namespace ipconnect
  {
    Rival::Rival(QObject *parent)
      : IRival(parent)
      , m_socket(nullptr)
      , m_server(nullptr)
      , m_me(Owner::NONE)
    {
      Kg::difficulty()->setEditable(false);
    }
    
    Rival::~Rival()
    {
      m_configWidget->setParent(0);
    }
    
    namespace
    {
      Owner itemToOwner(int index)
      {
        if (index == 0)
          return Owner::FIRST;
        
        return Owner::SECOND;
      }
    }
    
    void Rival::onDisconnected()
    {
      KMessageBox::information(0,
                i18n("Client has been disconnected. Please recreate a game"),
                i18n("Client has been disconnected"));
    }
    
    void Rival::requestGameConfig()
    {
      ClientConfig config;
      
      if (!m_configWidget->clientConfig(config))
      {
        return;
      }
      
      //Joining Game
      m_socket = new QTcpSocket(this);
      connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
      
      m_socket->connectToHost(config.m_host, config.m_port);

      kWarning() << "Connecting to the server...";
      if (m_socket->waitForConnected(5000))
      {
        kWarning() << "Connected";
        
        if (m_socket->waitForReadyRead())
        {
          kWarning() << "Reading Table config";
          const QByteArray& data = m_socket->readAll();
          kWarning() << "Data size" << data.size();
          QDataStream in(&const_cast<QByteArray&>(data), QIODevice::ReadOnly);
          QVariant variantData;
          
          quint32 me;
          in >> variantData >> me;
          m_me = static_cast<Owner>(me);
          
          if (!variantData.canConvert<GameConfig>())
          {
            kWarning() << "Cannot convert to GameConfig: "
                << variantData.typeName();
          }
          
          const GameConfig& config = variantData.value<GameConfig>();
          if (!config.isInititialized())
          {
            kWarning() << "Table config is invalid";
            return;
          }
          else
          {
            connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
            
            KMessageBox::information(0,
                i18n("Connected"),
                i18n("Good luck have fun"));
            emit needCreateBoard(config);
          }
        }
      }
      else
      {
        QMessageBox::warning(0,
            i18n("Warning!"),
            i18n("Cannot connect to the server: %1", m_socket->errorString()));
        
        emit needDestroy();
      }
    }
    
    void Rival::setBoardModel(BoardModel *board) //Is called after configureWidget
    {
      m_board = board;
      
      ServerConfig config;
      if (!m_configWidget->serverConfig(config))
      {
        // Client
        return;
      }
      
      m_me = itemToOwner(config.user);
      
      //Create server
      
      m_server = new QTcpServer(this);
      m_server->setMaxPendingConnections(1);

      connect(m_server, SIGNAL(newConnection()), this, SLOT(onNewConnectionHandle()));
      
      ConnectDialog dialog(m_server, config.m_port);
      if (dialog.exec() != QDialog::Accepted)
      {
        m_server->close();
        emit needDestroy();
      }
    }
    
    IConfigurationWidget* Rival::configureWidget()
    {
      m_configWidget.reset(new ConfigurationWidget);
      return m_configWidget.get();
    }

    Owner Rival::owner() const
    {
      return m_me;
    }

    void Rival::onPointAdded(const Point& point)
    {
      if (!m_socket)
        return;
      kWarning() << "Sending point";
      QByteArray array;
      QDataStream out(&array, QIODevice::WriteOnly);
      
      out << QVariant::fromValue<Point>(point);
      kWarning() << m_socket->write(array);
    }

    void Rival::onNewConnectionHandle()
    {
      m_socket = m_server->nextPendingConnection();
      
      if (!m_socket)
        return;
      
      QByteArray gameData;
      QDataStream out(&gameData, QIODevice::WriteOnly);
      out << QVariant::fromValue<GameConfig>(m_board->gameConfig())
          << static_cast<quint32>(StepQueue::other(m_me));
      m_socket->write(gameData);
      kWarning() << "Game config sent";
      connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
      connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    }

    void Rival::onReadyRead()
    {
      const QByteArray& array = m_socket->readAll();
      QDataStream in(&const_cast<QByteArray&>(array), QIODevice::ReadOnly);
      QVariant var;
      in >> var;
      const Point& point  = var.value<Point>();
      
      emit needAddPoint(point);
    }
    
    void Rival::onDifficultyChanged(const KgDifficultyLevel*)
    {
    }
  }
}

#include "rival.moc"
