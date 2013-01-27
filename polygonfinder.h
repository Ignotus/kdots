#pragma once

class QPolygon;
class QPoint;

template<class T>
class QList;

class PData;

template<class T>
class Matrix;

class PolygonFinder {
  public:
    PolygonFinder(const Matrix<PData>& matrix);
    virtual ~PolygonFinder();    
    virtual const QList<QPolygon>& findPolygons(const QPoint& first) = 0;
    
  protected:
    const Matrix<PData>& m_matrix;
};
