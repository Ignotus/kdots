#pragma once
#include <QDialog>

namespace Ui {
  class NewGameDialog;
}

class TableConfig;
class IPlugin;
class PluginFactory;
class NewGameDialog : public QDialog {
    Q_OBJECT
  public:
    NewGameDialog(QWidget *parent = 0);
    virtual ~NewGameDialog();
    
    void setPluginFactory(PluginFactory *factory);
  
  private slots:
    void onPluginSet(IPlugin *plugin);
    void showCreateTableWidget();
    
  private:
    Ui::NewGameDialog *m_ui;
    
  signals:
    void createTable(const TableConfig& config);
    void hidePluginFactory();
};