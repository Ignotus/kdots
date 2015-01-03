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
 *(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "mainwindow.hpp"
#include <QDir>
#include <KMenuBar>
#include <KStatusBar>
#include <KConfigDialog>
#include <KToolBar>
#include <KAction>
#include <KStandardAction>
#include <KActionCollection>
#include <interface/iplugin.hpp>
#include <interface/irival.hpp>
#include "ui_mainwindow.h"
#include "ui_boardconfigwidget.h"
#include "newgamedialog.hpp"
#include "boardview.hpp"
#include "pluginloader.hpp"
#include "kdots.h"
#include "stepqueue.hpp"
#include "graph.hpp"
#include "boardmodel.hpp"


namespace KDots
{
  MainWindow::MainWindow(QWidget *parent)
    : KXmlGuiWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_needDestroy(false)
    , m_gameConfiguring(false)
  {
    m_ui->setupUi(this);

    Kg::difficulty()->addStandardLevel(KgDifficultyLevel::Easy);
    Kg::difficulty()->addStandardLevel(KgDifficultyLevel::Medium);
    Kg::difficulty()->addStandardLevel(KgDifficultyLevel::Hard);
    
    KgDifficultyGUI::init(this);
    
    Kg::difficulty()->setEditable(false);

    initMenu();
    setupGUI(Default, "kdotsui.rc");
  }
  
  void MainWindow::initMenu()
  {
    KStandardAction::preferences(this, SLOT(onPreferences()), actionCollection());
    
    m_menu.m_newAction = new KAction(KIcon("file_new"), i18n("&New game"), this);
    m_menu.m_newAction->setShortcut(Qt::CTRL + Qt::Key_N);
    
    connect(m_menu.m_newAction, SIGNAL(triggered(bool)), this, SLOT(onNewGame())); 

    actionCollection()->addAction("NewGame", m_menu.m_newAction);
    
    m_menu.m_endAction = actionCollection()->addAction("EndGame", this, SLOT(endGame()));
    m_menu.m_endAction->setIcon(KIcon("window-close"));
    m_menu.m_endAction->setText(i18n("&End game"));
    m_menu.m_endAction->setShortcut(Qt::CTRL + Qt::Key_E);
    m_menu.m_endAction->setEnabled(false);
    
    m_menu.m_quitAction = actionCollection()->addAction("Quit", this, SLOT(close()));
    m_menu.m_quitAction->setIcon(KIcon("exit"));
    m_menu.m_quitAction->setText(i18n("&Quit"));
    m_menu.m_quitAction->setShortcut(Qt::CTRL + Qt::Key_Q);
    
    m_menu.m_undoAction = actionCollection()->addAction("UndoGame", this);
    m_menu.m_undoAction->setIcon(KIcon("undo"));
    m_menu.m_undoAction->setText(i18n("&Undo"));
    m_menu.m_undoAction->setEnabled(false);
    m_menu.m_undoAction->setShortcut(Qt::CTRL + Qt::Key_Z);
  }
  
  void MainWindow::endGame()
  {
    m_menu.m_endAction->setEnabled(false);
    m_model.reset();
    statusBar()->clearMessage();
  }
  
  void MainWindow::onNeedDestroy()
  {
    m_needDestroy = true;
    if (!m_gameConfiguring)
      endGame();
  }
  
  void MainWindow::onPreferences()
  {
    KConfigDialog dialog(this, i18n("Preferences"), Settings::self());
    
    QWidget *board = new QWidget(&dialog);
    
    Ui::BoardConfigWidget *boardUi = new Ui::BoardConfigWidget;
    boardUi->setupUi(board);
      
    dialog.addPage(board, i18n("Board"), QLatin1String("games-config-options"));
    
    if (dialog.exec() == QDialog::Accepted)
      emit preferencesUpdated();
  }
  
  namespace
  {
    std::unique_ptr<StepQueue> createStepQueue(const GameConfig& config)
    {
      if (config.m_mode == GameMode::DEFAULT_MODE)
        return std::unique_ptr<StepQueue>(new StepQueue(config.m_firstOwner));
      
      return std::unique_ptr<StepQueue>(new ExtraStepQueue(config.m_firstOwner));
    }
  }

  void MainWindow::onNewGame()
  {
    m_needDestroy = false;
    m_gameConfiguring = true;
    std::shared_ptr<void> finalizer(nullptr, [&](void*) { m_gameConfiguring = false; });
    NewGameDialog dialog;
    if (dialog.exec() != QDialog::Accepted)
      return;
    
    const GameConfig& config = dialog.gameConfig();
    
    if (!config.isInititialized())
      return;
    
    auto rival = std::move(dialog.rival());
    
    connect(Kg::difficulty(),
        SIGNAL(currentLevelChanged(const KgDifficultyLevel*)),
        rival.get(),
        SLOT(onDifficultyChanged(const KgDifficultyLevel*)));
    
    m_menu.m_undoAction->setEnabled(rival->canUndo());
    
    connect(rival.get(), SIGNAL(needDestroy()), this, SLOT(onNeedDestroy()));
    
    m_model = std::unique_ptr<BoardModel>(new BoardModel(config, createStepQueue(config)));
    connect(m_menu.m_undoAction, SIGNAL(triggered(bool)), m_model.get(), SLOT(undo()));
    connect(m_model.get(), SIGNAL(statusUpdated(const QString&)), statusBar(), SLOT(showMessage(const QString&)));
    
    {
    std::unique_ptr<IBoardView> view(new BoardView(this));
    
    connect(this, SIGNAL(preferencesUpdated()), view.get(), SLOT(update()));
    
    setCentralWidget(view.get());
    
    m_model->setView(std::move(view));
    }
    
    m_model->setRival(std::move(rival));
    
    m_menu.m_endAction->setEnabled(true);
    
    if (m_needDestroy)
      endGame();
  }
}

#include "mainwindow.moc"
