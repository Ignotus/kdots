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

#include "pluginmanagerwidget.hpp"
#include <QComboBox>
#include <QGridLayout>
#include "plugincontainer.hpp"

namespace KDots
{
	PluginManagerWidget::PluginManagerWidget (QWidget *parent)
		: QWidget (parent)
		, m_pluginComboBox (new QComboBox (this))
	{
		QGridLayout *layout = new QGridLayout (this);
		setLayout (layout);
		
		layout->addWidget (m_pluginComboBox, 0, 0);
		m_pluginComboBox->addItems (PluginContainer::instance ().plugins ().keys ());
	}

	QString PluginManagerWidget::pluginName () const
	{
		return m_pluginComboBox->currentText ();
	}
}