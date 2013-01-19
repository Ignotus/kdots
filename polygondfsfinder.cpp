#include "polygondfsfinder.h"

PolygonDFSFinder::PolygonDFSFinder(const Matrix<PData>& matrix)
: PolygonFinder(matrix) {
}
    
QVector<QPolygon> PolygonDFSFinder::findPolygons(const QPoint& first) {
  return QVector<QPolygon>();
}