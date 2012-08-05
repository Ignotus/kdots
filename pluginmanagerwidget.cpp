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
#include <interface/iplugin.hpp>
#include "plugincontainer.hpp"
#include "ui_pluginmanagerwidget.h"

namespace KDots
{
	PluginManagerWidget::PluginManagerWidget (QWidget *parent)
		: QWidget (parent)
		, m_ui (new Ui::PluginManagerWidget)
	{
		m_ui->setupUi (this);
		
		for (IPlugin *plugin : PluginContainer::instance ().plugins ().values ())
			m_ui->PluginComboBox->addItem (plugin->icon (), plugin->name ());
		
		onIndexChanged (0);
		
		connect (m_ui->PluginComboBox,
				SIGNAL (currentIndexChanged (int)),
				this,
				SLOT (onIndexChanged (int)));
	}
	
	void PluginManagerWidget::onIndexChanged (int current)
	{
		IPlugin *first = PluginContainer::instance ().plugin (m_ui->PluginComboBox->itemText (current));
		m_ui->Description->setText (first->description ());
	}

	QString PluginManagerWidget::pluginName () const
	{
		return m_ui->PluginComboBox->currentText ();
	}
}