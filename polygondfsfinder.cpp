#include "polygondfsfinder.h"
#include "matrix.h"
#include "pdata.h"

PolygonDFSFinder::PolygonDFSFinder(const Matrix<PData>& matrix)
: PolygonFinder(matrix) {
}

PolygonDFSFinder::~PolygonDFSFinder() {
}

bool PolygonDFSFinder::contains(const QPolygon& owner, const QPolygon& subject) {
  foreach (const QPoint& point, subject) {
    if (!owner.containsPoint(point, Qt::WindingFill)) {
      return false;
    }
  }
  
  return true;
}

const QList<QPolygon>& PolygonDFSFinder::findPolygons(const QPoint& first) {
  m_first = first;
  m_owner = m_matrix[first].owner();
  
  m_acc.clear();
  
  Matrix<char> matrixCache(m_matrix.size(), 0);
  QPolygon pointQueue;
  
  recursiveFind(first, QPoint(-1, -1), matrixCache, pointQueue);
  
  return m_acc;
}

void PolygonDFSFinder::recursiveFind(const QPoint& point,
                                     const QPoint& previous,
                                     Matrix<char>& matrixCache,
                                     QPolygon& pointQueue) {
  matrixCache[point] = 1;
  pointQueue.push_back(point);
  
  const QSize& ms = m_matrix.size();
  for (std::size_t k = 0; k < DIRECTION_COUNT; ++k) {
    const QPoint newPoint(point.x() + DX[k], point.y() + DY[k]);
    if (!isValid(newPoint, ms)) {
      continue;
    }
    
    const PData& pd = m_matrix[newPoint];
    
    if (pd.owner() != m_owner || pd.isCaptured()) {
      continue;
    }
    
    if (newPoint == previous) {
      continue;
    }
    
    if (pointQueue.size() > 3)
    {
      if (newPoint == m_first) {
        m_acc << pointQueue;
        continue;
      } else if (matrixCache[newPoint]) {
        continue;
      }
    }
    
    recursiveFind(newPoint, point, matrixCache, pointQueue);
  }
  
  pointQueue.pop_back();
}
