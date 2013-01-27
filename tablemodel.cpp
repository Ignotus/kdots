#include <QPoint>
#include <QDebug>

#include "tablemodel.h"
#include "ownerdetector.h"
#include "polygonfinder.h"

TableModel::TableModel(const QSize& size, QObject *parent)
: QObject(parent)
, m_data(size)
, m_ownerDetector(NULL)
, m_polygonFinder(NULL)
, m_lastPoint(-1, -1) {
}

TableModel::~TableModel() {
}

void TableModel::setOwnerDetector(OwnerDetector *detector) {
  m_ownerDetector = detector;
}

void TableModel::setPolygonFinder(PolygonFinder *finder) {
  m_polygonFinder = finder;
}

const Matrix<PData>& TableModel::data() const {
  return m_data;
}

int TableModel::ownerCount() const {
  return m_ownerDetector->owners();
}

const QPoint& TableModel::lastPoint() const {
  return m_lastPoint;
}

void TableModel::putPoint(const QPoint& point) {
  PData& dot = m_data[point];
  
  if (dot.owner() || dot.isCaptured())
    return;
  
  const int current = m_ownerDetector->owner();
  dot.setOwner(current);
  
  const bool isCaptured = findCapturedBorders(point);
  if (isCaptured) {
    dot.changeToBorder();
  }
  
  m_ownerDetector->nextOwner(isCaptured);
  emit dataChanged();
}

bool TableModel::findCapturedBorders(const QPoint& point) {
  const QList<QPolygon>& polygons = m_polygonFinder->findPolygons(point);
  
  const QSize& ms = m_data.size();
  const int currentOwn = m_ownerDetector->owner();
  
  bool foundBorders = false;
  QList<QPoint> capturedPoints;
  foreach (const QPolygon& polygon, polygons) {
    bool hasPoint = false;
    for (int i = 0, xmax = ms.width(); i < xmax; ++i) {
      for (int j = 0, ymax = ms.height(); j < ymax; ++j) {
        const QPoint currentPoint(i, j);
        PData& point = m_data[currentPoint];
        if (point.owner() && point.owner() != currentOwn && !point.isCaptured()) {
          if (polygon.containsPoint(currentPoint, Qt::OddEvenFill)) {
            capturedPoints << currentPoint;
            hasPoint = true;
          }
        }
      }
    }
    
    if (!hasPoint)
      continue;
    
    foundBorders = true;
    
    for (QPolygon::const_iterator it = polygon.begin(), end = polygon.end();
         it != end;
         ++it) {
      const QPoint& first = *it;
      const QPoint& second = next(it, polygon);
      
      m_data[first].addBorderTo(first, second);
      m_data[second].addBorderTo(second, first);
    }
      
    foreach (const QPoint& point, polygon) {
      m_data[point].changeToBorder();
      for (int i = 0, xmax = ms.width(); i < xmax; ++i) {
        for (int j = 0, ymax = ms.height(); j < ymax; ++j) {
          const QPoint currentPoint(i, j);
          PData& point = m_data[currentPoint];
          if (!point.owner() && !point.isCaptured()) {
            if (polygon.containsPoint(currentPoint, Qt::OddEvenFill)) {
              point.capture();
            }
          }
        }
      }
    }
  }
  
  foreach (const QPoint& point, capturedPoints) {
    m_data[point].capture();
  }
  
  return foundBorders;
}
