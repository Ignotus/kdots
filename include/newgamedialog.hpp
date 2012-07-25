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
#ifndef NEWGAMEDIALOG_H
#define NEWGAMEDIALOG_H
#include <QDialog>
#include "point.hpp"
#include "constants.hpp"

namespace Ui
{
  class NewGameDialog;
}

namespace KDots
{
  struct GameConfig
  {
    int m_width, m_height;
    GameMode m_mode;
    Owner m_firstOwner;
  };
  
  class NewGameDialog : public QDialog
  {
    Q_OBJECT
  public:
    NewGameDialog (QWidget *parent = 0);

    int getHeight () const;
    int getWidth () const;
    GameMode getGameMode () const;
    Owner getFirstMoving () const;
    QString getRival () const;

    GameConfig getGameConfig () const;
  private:
    Ui::NewGameDialog *m_ui;
  };
}


#endif
