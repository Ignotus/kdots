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
#include "newgamewidget.hpp"
#include "ui_newgamewidget.h"
#include <QDebug>
#include <QMetaType>
#include "constants.hpp"
#include "plugincontainer.hpp"

namespace KDots
{
	NewGameWidget::NewGameWidget (QWidget *parent)
		: QWidget (parent)
		, m_ui (new Ui::NewGameWidget)
	{
		m_ui->setupUi (this);
		
		
		//const QVariant& var = QVariant::fromValue<GameConfig> (getGameConfig ());
	}

	int NewGameWidget::getHeight () const
	{
		return m_ui->HeightSpinBox->value ();
	}

	int NewGameWidget::getWidth () const
	{
		return m_ui->WidthSpinBox->value ();
	}

	GameMode NewGameWidget::getGameMode () const
	{
		return m_ui->GameMode->currentIndex () ? GameMode::EXTRA_TURN_MODE : GameMode::DEFAULT_MODE;
	}

	Owner NewGameWidget::getFirstMoving () const
	{
		return m_ui->FirstMoving->currentIndex () ? Owner::SECOND : Owner::FIRST;
	}

	GameConfig NewGameWidget::getGameConfig () const
	{
		GameConfig config;
		config.m_firstOwner = getFirstMoving ();
		config.m_height = getHeight ();
		config.m_width = getWidth ();
		config.m_mode = getGameMode ();

		return config;
	}
	
/*	QDataStream&  operator<< (QDataStream& out, const KDots::GameConfig& config)
	{
		out << (int) config.m_firstOwner << config.m_height << config.m_width << (int) config.m_mode;
		return out;
	}
		
	QDataStream& operator>> (QDataStream& in, KDots::GameConfig& config)
	{
		int owner, mode;
		in >> owner >> config.m_height >> config.m_width >> mode;
		config.m_firstOwner = static_cast<Owner> (owner);
		config.m_mode = static_cast<GameMode> (mode);
		return in;
	}
	*/
}