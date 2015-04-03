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
#include "newgamewidget.hpp"

#include <QMetaType>

#include <constants.hpp>

#include "ui_newgamewidget.h"

#include "kdots.h"
#include "pluginloader.hpp"

namespace KDots
{
  NewGameWidget::NewGameWidget(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::NewGameWidget)
  {
    m_ui->setupUi(this);
    
    m_ui->HeightSpinBox->setValue(Settings::lastHeight());
    m_ui->WidthSpinBox->setValue(Settings::lastWidth());
    m_ui->GameMode->setCurrentIndex(Settings::lastGameMode());
    m_ui->FirstMoving->setCurrentIndex(Settings::lastFirstMoving());
  }

  int NewGameWidget::getHeight() const
  {
    Settings::setLastHeight(m_ui->HeightSpinBox->value());
    return Settings::lastHeight();
  }

  int NewGameWidget::getWidth() const
  {
    Settings::setLastWidth(m_ui->WidthSpinBox->value());
    return Settings::lastWidth();
  }

  GameMode NewGameWidget::getGameMode() const
  {
    Settings::setLastGameMode(m_ui->GameMode->currentIndex());
    return Settings::lastGameMode() ? GameMode::EXTRA_TURN_MODE : GameMode::DEFAULT_MODE;
  }

  Owner NewGameWidget::getFirstMoving() const
  {
    Settings::setLastFirstMoving(m_ui->FirstMoving->currentIndex());
    return Settings::lastFirstMoving() ? Owner::SECOND : Owner::FIRST;
  }

  GameConfig NewGameWidget::getGameConfig() const
  {
    GameConfig config;
    config.m_firstOwner = getFirstMoving();
    config.m_height = getHeight();
    config.m_width = getWidth();
    config.m_mode = getGameMode();
    
    return config;
  }
  
}
