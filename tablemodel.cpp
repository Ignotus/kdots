#include <QPoint>

#include "tablemodel.h"
#include "ownerdetector.h"
#include "polygonfinder.h"

TableModel::TableModel(const QSize& size, QObject *parent)
: QObject(parent)
, m_size(size)
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

const QSize& TableModel::size() const {
  return m_size;
}

const Matrix<PData>& TableModel::data() const {
  return m_data;
}

const QPoint& TableModel::lastPoint() const {
  return m_lastPoint;
}

void TableModel::putPoint(const QPoint& point) {
  PData& dot = m_data[point];
  
  if (dot.owner())
    return;
  
  const int current = m_ownerDetector->owner();
  dot.setOwner(current);
  
  bool isCaptured;
  if (isCaptured = findCapturedBorders(point)) {
    dot.changeToBorder();
  }
  
  qDebug() << m_ownerDetector->nextOwner(isCaptured);
  emit dataChanged();
}

bool TableModel::findCapturedBorders(const QPoint& point) {
  const QVector<QPolygon>& polygons = m_polygonFinder->findPolygons(point);
  
  const QSize& ms = size();
  const int currentOwn = m_ownerDetector->owner();
  
  bool foundBorders = false;
  foreach (const QPolygon& polygon, polygons) {
    bool hasPoint = false;
    for (int i = 0, xmax = ms.width(); i < xmax; ++i) {
      for (int j = 0, ymax = ms.height(); j < ymax; ++j) {
        PData& point = m_data[i][j];
        if (point.owner() && point.owner() != currentOwn && !point.isCaptured()) {
          if (polygon.containsPoint(QPoint(i, j), Qt::OddEvenFill)) {
            point.capture();
            hasPoint = true;
          }
        }
      }
    }
    
    if (hasPoint) {
      foundBorders = true;
      
      foreach (const QPoint& point, polygon) {
        m_data[point].changeToBorder();
      }
    }
  }
  
  return foundBorders;
}