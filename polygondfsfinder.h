#pragma once
#include <QPoint>
#include <QList>
#include "polygonfinder.h"

class QPolygon;

template<class T>
class Matrix;

class PolygonDFSFinder : public PolygonFinder {
  public:
    PolygonDFSFinder(const Matrix<PData>& matrix);
    virtual ~PolygonDFSFinder();

    const QList<QPolygon>& findPolygons(const QPoint& first);
    
  private:
    void recursiveFind(const QPoint& point, Matrix<char>& matrixCache, QPolygon& pointQueue);
    void resizeLastPolygon();
    
    bool contains(const QPolygon& owner, const QPolygon& subject);
    
  private:
    QList<QPolygon> m_acc;
    QPoint m_first;
    int m_owner;
};