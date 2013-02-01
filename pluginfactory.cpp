#include <QDebug>
#include <QStandardItemModel>

#include <KService>
#include <KServiceTypeTrader>
#include <KIcon>

#include "pluginfactory.h"
#include "interfaces/iplugin.h"

PluginFactory::PluginFactory(QObject *parent)
: QObject(parent) {
}

PluginFactory::~PluginFactory() {
}

const QMap<QString, KService::Ptr>& PluginFactory::allInfo() const {
  return m_metadata;
}

QAbstractItemModel* PluginFactory::itemModel(QObject *parent) {
  if (parent == 0)
    parent = this;
  
  QStandardItemModel *model = new QStandardItemModel(parent);
  
  foreach (const KService::Ptr& data, m_metadata) {
    model->appendRow(new QStandardItem(KIcon(data->icon()), data->name()));
  }
  
  return model;
}

KService::Ptr PluginFactory::info(const QString& name) const {
  QMap<QString, KService::Ptr>::const_iterator it = m_metadata.find(name);
  if (it == m_metadata.end())
    return KService::Ptr();
  
  return *it;
}

void PluginFactory::emitCurrentPlugin() {
  emit pluginSet(currentPlugin());
}

void PluginFactory::setCurrentPlugin(const QString& name) {
  m_currentPluginName = name;
}

IPlugin* PluginFactory::currentPlugin() {
  QMap<QString, IPlugin*>::const_iterator it = m_plugins.find(m_currentPluginName);
  if (it == m_plugins.end())
    return NULL;
  
  return *it;
}

QList<QString> PluginFactory::availablePlugins() const {
  return m_plugins.keys();
}

void PluginFactory::loadPlugins() {
  KService::List offers = KServiceTypeTrader::self()->query("KDots/Plugin");
  
  foreach (const KService::Ptr& shared, offers) {
    m_metadata[shared->name()] = shared;
  }
 
  KService::List::const_iterator iter;
  for(iter = offers.begin(); iter < offers.end(); ++iter) {
    KService::Ptr service = *iter;
    
    KPluginFactory *factory = KPluginLoader(service->library()).factory();
 
    if (!factory) {
      qWarning() << Q_FUNC_INFO
                 << "Could not load the plugin:" << service->library();
      continue;
    }
 
    IPlugin *plugin = factory->create<IPlugin>(this);
 
    if (plugin) {
      qDebug() << Q_FUNC_INFO <<  "Load plugin:" << service->name() << plugin;
      m_plugins[service->name()] = plugin;
    } else {
      qDebug() << Q_FUNC_INFO << "Cannot load a plugin";
    }
  }
}