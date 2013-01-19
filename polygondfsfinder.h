#pragma once
#include "polygonfinder.h"

class PolygonDFSFinder : public PolygonFinder {
  public:
    PolygonDFSFinder(const Matrix<PData>& matrix);
    virtual ~PolygonDFSFinder();

    const QList<QPolygon>& findPolygons(const QPoint& first);
    
  private:
    QList<QPolygon> m_acc;
};
