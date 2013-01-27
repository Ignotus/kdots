#include <pluginmacros.h>
#include "plugin.h"

KDOTS_PLUGIN_EXPORT(Plugin)

Plugin::Plugin(QObject *parent, const QVariantList& varlist)
: IPlugin(parent, varlist) {
}

QString Plugin::name() const {
  return "Single PC";
}
  
QString Plugin::description() const {
  return "Playing KDots";
}
  
IRival* Plugin::createRival() const {
  return NULL;
}