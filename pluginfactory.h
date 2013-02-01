#pragma once
#include <memory>

#include <QObject>
#include <QMap>

#include <KService>

class IPlugin;
class QAbstractItemModel;
class PluginFactory : public QObject {
    Q_OBJECT
    
  public:
    PluginFactory(QObject *parent = 0);
    virtual ~PluginFactory();
    
    void loadPlugins();
    
    QAbstractItemModel* itemModel(QObject* parent = 0);
    
    QList<QString> availablePlugins() const;
    const QMap<QString, KService::Ptr>& allInfo() const;
    KService::Ptr info(const QString& name) const;
    
    IPlugin* currentPlugin();
   
  public slots:
    void setCurrentPlugin(const QString& name);
    void emitCurrentPlugin();
    
  signals:
    void pluginSet(IPlugin *plugin);
    
  private:
    QMap<QString, IPlugin*> m_plugins;
    QMap<QString, KService::Ptr> m_metadata;
    QString m_currentPluginName;
};