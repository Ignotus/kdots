#include "polygondfsfinder.h"

PolygonDFSFinder::PolygonDFSFinder(const Matrix<PData>& matrix)
: PolygonFinder(matrix) {
}

PolygonDFSFinder::~PolygonDFSFinder() {
}

const QList<QPolygon>& PolygonDFSFinder::findPolygons(const QPoint& first) {
  m_acc.clear();
  
  
  return m_acc;
}
