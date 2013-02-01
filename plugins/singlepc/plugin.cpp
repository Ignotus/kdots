#include <QDebug>
#include <pluginmacros.h>
#include "plugin.h"

KDOTS_PLUGIN_EXPORT(Plugin)

Plugin::Plugin(QObject *parent, const QVariantList& varlist)
: IPlugin(parent, varlist) {
}

void Plugin::configure(QObject *owner) {
  emit createTable();
}