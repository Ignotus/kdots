#include "polygondfsfinder.h"
#include "matrix.h"
#include "pdata.h"

PolygonDFSFinder::PolygonDFSFinder(const Matrix<PData>& matrix)
: PolygonFinder(matrix) {
}

PolygonDFSFinder::~PolygonDFSFinder() {
}

const QList<QPolygon>& PolygonDFSFinder::findPolygons(const QPoint& first) {
  m_first = first;
  m_owner = m_matrix[first].owner();
  
  m_acc.clear();
  m_pointCache.clear();
  m_pointCache << first;
  
  Matrix<char> matrixCache(m_matrix.size(), 0);
  matrixCache[first] = 1;
  
  m_pointCache << first;
  
  recursiveFind(first, matrixCache);
  
  return m_acc;
}

void PolygonDFSFinder::recursiveFind(const QPoint& point, Matrix< char >& matrixCache) {
  const QSize& ms = m_matrix.size();
  
  matrixCache[point] = 1;
  for (int k = 0; k < DIRECTION_COUNT; ++k) {
    const QPoint newPoint(point.x() + DX[k], point.y() + DY[k]);
    
    if (newPoint.x() < 0 || newPoint.y() < 0
        || newPoint.x() >= ms.width() || newPoint.y() >= ms.height()) {
      continue;
    }
    
    const PData& pd = m_matrix[newPoint];
    
    if (pd.owner() != m_owner || pd.isCaptured()) {
      continue;
    }
    
    if (newPoint == m_first) {
      m_acc << m_pointCache;
      continue;
    }
    
    if (matrixCache[newPoint]) {
      continue;
    }
    
    m_pointCache.push_back(newPoint);
    recursiveFind(newPoint, matrixCache);
  }
  
  m_pointCache.pop_back();
}
