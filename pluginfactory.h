#pragma once
#include <memory>

#include <QObject>
#include <QMap>

#include <KService>

class IPlugin;
class PluginFactory : public QObject {
    Q_OBJECT
    
  public:
    PluginFactory(QObject *parent = 0);
    virtual ~PluginFactory();
    
    void loadPlugins();
    
    QList<QString> availablePlugins() const;
    const KService* info(const QString& name) const;
    const IPlugin* plugin(const QString& name) const;
    
  private:
    QMap<QString, IPlugin*> m_plugins;
    QMap<QString, KService::Ptr> m_metadata;
};