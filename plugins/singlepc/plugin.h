#pragma once
#include <iplugin.h>

class KDE_EXPORT Plugin : public IPlugin {
  public:
    Plugin(QObject *parent, const QVariantList& varlist);
    
    void configure(QObject *owner = 0);
};