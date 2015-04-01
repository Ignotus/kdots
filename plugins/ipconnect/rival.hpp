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
#pragma once
#include <memory>
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <interface/irival.hpp>

namespace KDots
{
  class BoardModel;
  namespace ipconnect
  {
    class ConfigurationWidget;
    class Rival : public KDots::IRival
    {
      Q_OBJECT
    public:
      Rival(QObject *parent = 0);
      virtual ~Rival();
      
      IConfigurationWidget* configureWidget();

      void setBoardModel(BoardModel *board);
      void requestGameConfig();

      Owner owner() const;

    public Q_SLOTS:
      void onPointAdded(const Point& point);
      void onDifficultyChanged(const KgDifficultyLevel*);
      
    private Q_SLOTS:
      void onNewConnectionHandle();
      void onReadyRead();
      void onDisconnected();
      
    Q_SIGNALS:
      void needCreateBoard(const GameConfig& config);
      void needDestroy();
      void needAddPoint(const Point&);
      
    private:
      BoardModel *m_board;
      QTcpSocket *m_socket;
      QTcpServer *m_server;
      
      Owner m_me;
      
      std::unique_ptr<ConfigurationWidget> m_configWidget;
    };
  }
}
