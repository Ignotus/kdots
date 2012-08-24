/*
 * KDots
 * Copyright (c) 2011-2012 Minh Ngo <nlminhtl@gmail.com>
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
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "pluginmanagerwidget.hpp"
#include <interface/iplugin.hpp>
#include "kdots.h"
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
		
		m_ui->PluginComboBox->setCurrentIndex (Settings::lastPlugin ());
		onIndexChanged (Settings::lastPlugin ());
		
		connect (m_ui->PluginComboBox,
				SIGNAL (currentIndexChanged (int)),
				this,
				SLOT (onIndexChanged (int)));
	}
	
	void PluginManagerWidget::onIndexChanged (int current)
	{
		IPlugin *first = PluginContainer::instance ().plugin (m_ui->PluginComboBox->itemText (current));
		m_ui->Description->setText (first->description ());
		
		Settings::setLastPlugin (current);
	}

	QString PluginManagerWidget::pluginName () const
	{
		return m_ui->PluginComboBox->currentText ();
	}
}

#include "include/pluginmanagerwidget.moc"