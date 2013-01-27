#include <QDebug>
#include <QVector>

#include "pointcounter.h"

PointCounter::PointCounter(int owners, QObject* parent)
: QObject(parent)
, m_marks(owners, 0) {
}

void PointCounter::onPointCaptured(int owner, int count) {
  m_marks[owner - 1] += count;
  
  for (int i = 0; i < m_marks.size(); ++i) {
    qDebug() << Q_FUNC_INFO << "Player #" << i + 1<< ":" << m_marks[i];
  }
}