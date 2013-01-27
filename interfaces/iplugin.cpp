#include "iplugin.h"

IPlugin::IPlugin(QObject *parent, const QVariantList&)
: QObject(parent) {
}

IPlugin::~IPlugin() {}