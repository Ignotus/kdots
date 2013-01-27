#pragma once
#include <QObject>

class PluginLoader : public QObject {
    Q_OBJECT
    
  public:
    PluginLoader(QObject *parent = 0);
    virtual ~PluginLoader();
    
    void loadPlugins();
};