#include <QApplication>
#include <QDebug>
#include "tablemodel.h"
#include "tableview.h"
#include "ownerdetector.h"
#include "polygondfsfinder.h"

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  
  TableView view;
  TableModel model(QSize(10, 10));
  
  AdditionalMove detector(2, 1);
  model.setOwnerDetector(&detector);
  
  PolygonDFSFinder finder(model.data());
  model.setPolygonFinder(&finder);
  
  view.setModel(&model);
  
  view.show();
  
  return app.exec();
}