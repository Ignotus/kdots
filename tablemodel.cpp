#include <QPoint>

#include "tablemodel.h"

TableModel::TableModel(const QSize& size, QObject *parent)
: QObject(parent)
, m_size(size)
, m_data(size) {
}

TableModel::~TableModel() {
}

const QSize& TableModel::size() const {
  return m_size;
}

const Matrix<PData>& TableModel::data() const {
  return m_data;
}

void TableModel::putPoint(const QPoint& point) {
  PData newData;
  m_data[point] = newData;
}