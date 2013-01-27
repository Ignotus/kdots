#include "matrix.h"

bool isValid(const QPoint& point, const QSize& ms) {
  return (point.x() >= 0 && point.x() < ms.width()
          && point.y() >= 0 && point.y() < ms.height());
}

QPoint next(const QPolygon::const_iterator& it,const QPolygon& poly) {
  const QPolygon::const_iterator ifend = it + 1;
  if (ifend == poly.end())
    return *poly.begin();
    
  return *ifend;
}