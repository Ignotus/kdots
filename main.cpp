#include <QApplication>
#include "tablemodel.h"
#include "tableview.h"
#include "ownerdetector.h"
#include "polygondfsfinder.h"
#include "pointcounter.h"

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
  
  PointCounter counter(model.ownerCount());
  
  QObject::connect(&model, SIGNAL(pointCaptured(int, int)), &counter, SLOT(onPointCaptured(int, int)));
          
  
  return app.exec();
}