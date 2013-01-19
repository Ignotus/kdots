#pragma once
#include "polygonfinder.h"

class PolygonDFSFinder : public PolygonFinder {
  public:
    PolygonDFSFinder(const Matrix<PData>& matrix);
    
    QVector<QPolygon> findPolygons(const QPoint& first);
};