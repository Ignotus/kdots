#pragma once
#include <QVector>
#include <QPolygon>

class PData;

template<class T>
class Matrix;

class PolygonFinder {
  public:
    PolygonFinder(const Matrix<PData>& matrix);
    
    virtual QVector<QPolygon> findPolygons(const QPoint& first) = 0;
    
  protected:
    const Matrix<PData>& m_matrix;
};