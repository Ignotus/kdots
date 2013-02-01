#pragma once
#include <memory>

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class PluginFactory;
class OwnerDetector;
class PolygonFinder;
class TableConfig;
class IPlugin;
class MainWindow : public QMainWindow {
    Q_OBJECT
    
  public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
  
  public slots:
    void init();
    
  private slots:
    void onCreateTable(const TableConfig& config);
    void onPluginSet(IPlugin *plugin);
    
  private:
    OwnerDetector* createOwnerDetector(int type, int firstPlayer) const;
  private:
    Ui::MainWindow *m_ui;
    PluginFactory *m_loader;
    std::unique_ptr<IPlugin> m_plugin;
};