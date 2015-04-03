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
#include "newgamedialog.hpp"

#include <QDebug>

#include <interface/irival.hpp>
#include <interface/iplugin.hpp>
#include <interface/iconfigurationwidget.hpp>

#include "ui_newgamedialog.h"

#include "pluginmanagerwidget.hpp"
#include "newgamewidget.hpp"
#include "pluginloader.hpp"

namespace KDots
{
  NewGameDialog::NewGameDialog(QWidget *parent)
    : QDialog(parent)
    , m_ui(new Ui::NewGameDialog)
    , m_game(nullptr)
    , m_pluginManager(new PluginManagerWidget(this))
    , m_configWidget(nullptr)
    , m_needCreateBoard(false)
  {
    m_ui->setupUi(this);
      
    m_ui->Grid->addWidget(m_pluginManager, 0, 0);
    
    connect(m_ui->NextButton, SIGNAL(clicked(bool)), this, SLOT(pluginWidget()));
    m_ui->NextButton->setFocus();
  }
  
  NewGameDialog::~NewGameDialog()
  {
  }
  
  std::unique_ptr<IRival> NewGameDialog::rival()
  {
    return std::move(m_rival);
  }
  
  void NewGameDialog::accept()
  {
    if (m_game)
      m_config = m_game->getGameConfig();
    else if (m_needCreateBoard)
    {
      m_rival->requestGameConfig();
    }
    
    if (m_configWidget)
      m_configWidget->setParent(0);
    
    QDialog::accept();
  }
  
  const GameConfig& NewGameDialog::gameConfig() const
  {
    return m_config;
  }
  
  void NewGameDialog::onBoardConfigReceived(const GameConfig& config)
  {
    m_config = config;
  }
  
  void NewGameDialog::pluginWidget()
  {
    m_ui->NextButton->disconnect(this, SLOT(pluginWidget()));
    
    if (!m_pluginManager)
    {
      qWarning() << "Cannot cast to PluginManagerWidget";
      return;
    }
    
    const QString& pluginName = m_pluginManager->pluginName();
    
    IPlugin *pluginInstance = PluginLoader::instance().plugin(pluginName);
    if (!pluginInstance)
    {
      qDebug() << "Plugin instance doesn't exists";
      return;
    }
    
    m_rival = std::move(pluginInstance->createRival());
    
    connect(m_rival.get(),
            SIGNAL(needCreateBoard(const GameConfig&)),
            this,
            SLOT(onBoardConfigReceived(const GameConfig&)));
    
    m_pluginManager->hide();
    
    m_configWidget = m_rival->configureWidget();
    
    if (m_configWidget == nullptr)
    {
      gameWidget();
      return;
    }
    
    m_ui->Grid->addWidget(m_configWidget , 0, 0);
    
    connect(m_configWidget, SIGNAL(needCreateBoard(bool)), this, SLOT(onNeedCreateBoard(bool)));
    
    connect(m_ui->NextButton, SIGNAL(clicked(bool)), this, SLOT(gameWidget()));
    
    m_configWidget->requestState();
  }
  
  void NewGameDialog::onNeedCreateBoard(bool val)
  {
    m_needCreateBoard = !val;
    m_ui->NextButton->setEnabled(val);
    m_ui->OKButton->setEnabled(m_needCreateBoard);
    if (val)
      connect(m_ui->NextButton, SIGNAL(clicked(bool)), this, SLOT(gameWidget()));
    else
      m_ui->NextButton->disconnect(this, SLOT(gameWidget()));
  }
  
  void NewGameDialog::gameWidget()
  {
    if (m_configWidget)
      m_configWidget->hide();
    
    m_ui->NextButton->setEnabled(false);
    m_ui->OKButton->setEnabled(true);
    m_ui->OKButton->setFocus();
    m_game = new NewGameWidget(this);
    m_ui->Grid->addWidget(m_game, 0, 0);
  }
}

#include "moc_newgamedialog.cpp"
