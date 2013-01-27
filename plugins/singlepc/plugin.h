#pragma once
#include <iplugin.h>

class KDE_EXPORT Plugin : public IPlugin {
  public:
    Plugin(QObject *parent, const QVariantList& varlist);
    QString name() const;
  
    QString description() const;
  
    IRival* createRival() const;
};