#include <QPoint>

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
  foreach (const QPolygon& polygon, polygons) {
    bool hasPoint = false;
    for (int i = 0, xmax = ms.width(); i < xmax; ++i) {
      for (int j = 0, ymax = ms.height(); j < ymax; ++j) {
        const QPoint currentPoint(i, j);
        PData& point = m_data[currentPoint];
        if (point.owner() && point.owner() != currentOwn && !point.isCaptured()) {
          if (polygon.containsPoint(currentPoint, Qt::OddEvenFill)) {
            point.capture();
            hasPoint = true;
            
            qDebug() << "Has point: " << currentPoint << " in ";
            qDebug() << polygon;
          }
        }
      }
    }
    
    if (hasPoint) {
      foundBorders = true;
      
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
  }
  
  return foundBorders;
}
