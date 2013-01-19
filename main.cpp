#include <QApplication>
#include <QDebug>
#include "tablemodel.h"
#include "tableview.h"
#include "ownerdetector.h"

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  
  TableView view;
  TableModel model(QSize(5, 5));
  
  AdditionalMove detector(2, 1);
  
  model.setOwnerDetector(&detector);
  
  view.setModel(&model);
  
  view.show();
  
  return app.exec();
}