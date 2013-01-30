#pragma once
#include <QWidget>

namespace Ui {
  class PluginFactoryView;
}

class QModelIndex;

class PluginFactory;
class PluginFactoryView : public QWidget {
    Q_OBJECT
    
  public:
    PluginFactoryView(QWidget *parent = 0);
    virtual ~PluginFactoryView();
    
    void setModel(PluginFactory *factory);
    
  private slots:
    void onIndexChanged(const QModelIndex& current);
  
  signals:
    void onCurrentPluginChanged(const QString& name);
    
  private:
    Ui::PluginFactoryView *m_ui;
    PluginFactory *m_factory;
};