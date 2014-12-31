/*
 * KDots
 * Copyright(c) 2011-2012 Minh Ngo <nlminhtl@gmail.com>
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
#include "pluginmanagerwidget.hpp"
#include <QStandardItemModel>
#include <interface/iplugin.hpp>
#include "kdots.h"
#include "pluginloader.hpp"
#include "pluginwidgetdelegate.hpp"
#include "ui_pluginmanagerwidget.h"

namespace KDots
{
  PluginManagerWidget::PluginManagerWidget(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::PluginManagerWidget)
  {
    m_ui->setupUi(this);
    
    QStandardItemModel *model = new QStandardItemModel(this);
    for (const QString& name : PluginLoader::instance().availablePlugins())
    {
      model->appendRow(new QStandardItem(PluginLoader::instance().plugin(name)->icon(),
                                         name));
    }
    
    m_ui->PluginList->setModel(model);
    m_ui->PluginList->setItemDelegate(new PluginWidgetDelegate(m_ui->PluginList));
    m_ui->PluginList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_ui->PluginList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(m_ui->PluginList,
        SIGNAL(clicked(const QModelIndex&)),
        this,
        SLOT(onIndexChanged(const QModelIndex&)));
    
    const QModelIndex& index =model->index(0, 0);
    m_ui->PluginList->selectionModel()->select(index, QItemSelectionModel::Select);
    onIndexChanged(index);
  }
  
  void PluginManagerWidget::onIndexChanged(const QModelIndex& current)
  {
    const QString& pluginName = current.data().toString();
    IPlugin *first = PluginLoader::instance().plugin(pluginName);
    m_ui->Description->setText(first->description());
    
    Settings::setLastPlugin(current.row());
  }

  QString PluginManagerWidget::pluginName() const
  {
    return m_ui->PluginList->selectionModel()->selectedRows()
        .at(0).data().toString();
  }
}

#include "pluginmanagerwidget.moc"