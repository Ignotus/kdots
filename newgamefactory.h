#pragma once
#include <QWidget>

namespace Ui {
  class NewGameFactory;
}

class TableConfig;
class NewGameFactory : public QWidget {
    Q_OBJECT
    
  public:
    NewGameFactory(QWidget *parent = 0);
    virtual ~NewGameFactory();
    
  public slots:
    void emitTableConfig();
    
  private:
    Ui::NewGameFactory *m_ui;
    
  signals:
    void createTable(const TableConfig& config);
};