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

#include "newgamedialog.hpp"
#include "ui_newgamedialog.h"
#include <QDebug>
#include "constants.hpp"
#include "plugincontainer.hpp"

namespace KDots
{
  NewGameDialog::NewGameDialog (QWidget *parent)
    : QDialog (parent)
    , m_ui (new Ui::NewGameDialog)
  {
    m_ui->setupUi (this);
    
    m_ui->Rival->addItems (PluginContainer::instance ().plugins ().keys ());
  }

  int
  NewGameDialog::getHeight () const
  {
    return m_ui->HeightSpinBox->value ();
  }

  int
  NewGameDialog::getWidth () const
  {
    return m_ui->WidthSpinBox->value ();
  }

  GameMode
  NewGameDialog::getGameMode () const
  {
    return m_ui->GameMode->currentIndex ()
        ? GameMode::EXTRA_TURN_MODE
        : GameMode::DEFAULT_MODE;
  }

  Owner
  NewGameDialog::getFirstMoving () const
  {
    return m_ui->FirstMoving->currentIndex () ? Owner::SECOND : Owner::FIRST;
  }
  
  QString
  NewGameDialog::getRival () const
  {
    return m_ui->Rival->currentText ();
  }
  
  GameConfig
  NewGameDialog::getGameConfig () const
  {
    GameConfig config;
    config.m_firstOwner = getFirstMoving ();
    config.m_height = getHeight ();
    config.m_width = getWidth ();
    config.m_mode = getGameMode ();
    
    return config;
  }
}