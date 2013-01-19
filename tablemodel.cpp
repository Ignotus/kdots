#include <QPoint>

#include "tablemodel.h"
#include "ownerdetector.h"

TableModel::TableModel(const QSize& size, QObject *parent)
: QObject(parent)
, m_size(size)
, m_data(size)
, m_ownerDetector(NULL)
, m_lastPoint(-1, -1) {
}

TableModel::~TableModel() {
}

void TableModel::setOwnerDetector(OwnerDetector *detector) {
  m_ownerDetector = detector;
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
  return false;
}