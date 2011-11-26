/*
 * kdots
 * Copyright (C) 2011 Minh Ngo <nlminhtl@gmail.com>
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

#include "newgamedialog.h"
#include "ui_newgamedialog.h"

NewGameDialog::NewGameDialog (QWidget *parent)
: QDialog (parent)
, ui (new Ui::NewGameDialog)
{
	ui->setupUi (this);
}

NewGameDialog::~NewGameDialog ()
{
	delete ui;
}

int NewGameDialog::getHeight () const
{
	return ui->HeightSpinBox->value ();
}

int NewGameDialog::getWidth () const
{
	return ui->WidthSpinBox->value ();
}
