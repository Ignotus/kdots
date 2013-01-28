#include <QDebug>
#include <KService>
#include <KServiceTypeTrader>
#include "pluginfactory.h"
#include "interfaces/iplugin.h"

PluginFactory::PluginFactory(QObject *parent)
: QObject(parent) {
}

PluginFactory::~PluginFactory() {
}

const QMap<QString, KService::Ptr>& PluginFactory::info() const {
  return m_metadata;
}

const IPlugin* PluginFactory::plugin(const QString& name) const {
  QMap<QString, IPlugin*>::const_iterator it = m_plugins.find(name);
  if (it == m_plugins.end())
    return NULL;
  
  return *it;
}

QList<QString> PluginFactory::availablePlugins() const {
  return m_plugins.keys();
}

void PluginFactory::loadPlugins() {
  KService::List offers = KServiceTypeTrader::self()->query("KDots/Plugin");
 
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
      qDebug() << Q_FUNC_INFO <<  "Load plugin:" << service->name();
      m_plugins[service->name()] = plugin;
      m_metadata[service->name()] = service;
    } else {
      qDebug() << Q_FUNC_INFO << "Cannot load a plugin";
    }
  }
}