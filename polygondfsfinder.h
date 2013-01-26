#pragma once
#include "polygonfinder.h"

template<class T>
class Matrix;

class PolygonDFSFinder : public PolygonFinder {
  public:
    PolygonDFSFinder(const Matrix<PData>& matrix);
    virtual ~PolygonDFSFinder();

    const QList<QPolygon>& findPolygons(const QPoint& first);
    
  private:
    void recursiveFind(const QPoint& point, Matrix<char>& matrixCache, QPolygon& pointQueue);
    
    bool contains(const QPolygon& owner, const QPolygon& subject);
    
  private:
    QList<QPolygon> m_acc;
    QPoint m_first;
    int m_owner;
};
