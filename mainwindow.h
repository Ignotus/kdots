#pragma once
#include <memory>

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class OwnerDetector;
class PolygonFinder;
class MainWindow : public QMainWindow {
    Q_OBJECT
    
  public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    void init();
    
  private:
    Ui::MainWindow *m_ui;
};