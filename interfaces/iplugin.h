#pragma once
#include <QString>
#include <QIcon>
#include <QVariant>

#include <KXMLGUIClient>

#include "pluginmacros.h"

class IRival;
class KDOTS_EXPORT IPlugin : public QObject {
    Q_OBJECT
    
  public:
    IPlugin(QObject *parent, const QVariantList&);

    virtual ~IPlugin();
};