#include <QDebug>
#include <KService>
#include <KServiceTypeTrader>
#include "pluginloader.h"
#include "interfaces/iplugin.h"

PluginLoader::PluginLoader(QObject *parent)
: QObject(parent) {
}

PluginLoader::~PluginLoader() {
}

void PluginLoader::loadPlugins() {
    KService::List offers = KServiceTypeTrader::self()->query("KDots/Plugin");
 
    KService::List::const_iterator iter;
    for(iter = offers.begin(); iter < offers.end(); ++iter)
    {
       QString error;
       KService::Ptr service = *iter;
 
        KPluginFactory *factory = KPluginLoader(service->library()).factory();
 
        if (!factory)
        {
            qWarning() << Q_FUNC_INFO
                       << "Could not load the plugin:" << service->library();
            continue;
        }
 
       IPlugin *plugin = factory->create<IPlugin>(this);
 
       if (plugin) {
           qDebug() << Q_FUNC_INFO <<  "Load plugin:" << service->name();
       } else {
           qDebug() << Q_FUNC_INFO << error;
       }
    }
}