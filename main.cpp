#include <QApplication>
#include "tablemodel.h"
#include "tableview.h"

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  
  TableView view;
  TableModel model(QSize(5, 5));
  
  view.setModel(&model);
  view.updateView();
  
  view.show();
  
  return app.exec();
}