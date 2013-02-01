#pragma once
#include <QString>
#include <QIcon>
#include <QVariant>

#include <KXMLGUIClient>

#include "pluginmacros.h"
#include "tableconfig.h"

class KDOTS_EXPORT IPlugin : public QObject {
    Q_OBJECT
    
  public:
    IPlugin(QObject *parent, const QVariantList&);

    virtual ~IPlugin();

    virtual void configure(QObject *owner = 0);
   
  private:
    void placeWidgetToOwner(QWidget *w, QObject *owner);
    
  public slots:
    void completeConfiguring();
    
  signals:
    void createTable();
    void createTableByConfig(const TableConfig& config);
};