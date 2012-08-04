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
#ifndef TABLEWIDGET_HPP
#define TABLEWIDGET_HPP
#include <memory>
#include <QWidget>
#include "constants.hpp"
#include "interface/irival.hpp"

namespace KDots
{
	 class DotTable;
	 class GameConfig;

	class TableWidget : public QWidget
	{
		Q_OBJECT

		DotTable *m_table;
		int m_height, m_width;

		std::shared_ptr<KDots::IRival> m_rival;
	public:
		TableWidget (const GameConfig& config, std::shared_ptr<IRival> rival,
				QWidget *parent = 0);
	protected:
		void mousePressEvent (QMouseEvent *event);
		void paintEvent (QPaintEvent *event);
	signals:
		void updateStatusBar (const QString& msg);
	};
}

#endif
