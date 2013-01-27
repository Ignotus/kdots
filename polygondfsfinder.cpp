#include <QPair>
#include <QDebug>

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
  
  recursiveFind(first, matrixCache, pointQueue);
  
  return m_acc;
}

void PolygonDFSFinder::resizeLastPolygon() {
  QPolygon& last = m_acc.back();
  
  const int own = m_matrix[m_first].owner();
  
  for (QPolygon::iterator it = last.begin(); it != last.end(); ++it) {
    const QPoint& first = *it;
    const QPoint& second = next(it, last);
    
    if (first.x() == second.x() || first.y() == second.y()) {
      continue;
    }
    
    const QPoint newPoints[] = {QPoint(first.x(), second.y()),
                                QPoint(second.x(), first.y())};
                                          
    for (std::size_t i = 0; i < sizeof(newPoints) / sizeof(QPoint); ++i) {
      const QPoint& current = newPoints[i];
      const PData& data = m_matrix[current];
      if (data.owner() != own || data.isCaptured()
          || last.containsPoint(current, Qt::OddEvenFill)) {
        continue;
      }
      
      it = last.insert(it + 1, current);
      
      break;
    }
  }
}

void PolygonDFSFinder::recursiveFind(const QPoint& point,
                                     Matrix<char>& matrixCache,
                                     QPolygon& pointQueue) {
  matrixCache[point] = 1;
  pointQueue.push_back(point);
  
  const QSize& ms = m_matrix.size();
  
  typedef QPair<QPoint, int> PriorityType;
  QVector<PriorityType> priorities;
  priorities.reserve(DIRECTION_COUNT);
  
  for (std::size_t k = 0; k < DIRECTION_COUNT; ++k) {
    const QPoint newPoint(point.x() + DX[k], point.y() + DY[k]);
    if (!isValid(newPoint, ms)) {
      priorities << PriorityType(newPoint, -1);
      continue;
    }
    
    if (pointQueue.size() > 3 && newPoint == m_first) {
      m_acc << pointQueue;
      resizeLastPolygon();
      priorities << PriorityType(newPoint, -1);
      continue;
    }
    
    const PData& pd = m_matrix[newPoint];
    
    if (pd.owner() != m_owner || pd.isCaptured() || matrixCache[newPoint]) {
      priorities << PriorityType(newPoint, -1);
      continue;
    }
    
    const int prio = std::abs(newPoint.x() - m_first.x()) + std::abs(newPoint.y() - m_first.y());
    
    priorities << PriorityType(newPoint, prio);
  }
  
  std::sort(priorities.begin(),
            priorities.end(),
            [](const PriorityType& first, const PriorityType& second) -> bool
            {
                return first.second > second.second;
            });
  
  foreach (const PriorityType& prio, priorities) {
    if (prio.second == -1)
      break;
    
    recursiveFind(prio.first, matrixCache, pointQueue);
  }
  
  
  
  pointQueue.pop_back();
}
