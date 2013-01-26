#include <stdexcept>

#include <QDebug>
#include <QPen>

#include "configuration.h"

Configuration::Configuration()
: m_pens(NULL)
, m_brush(NULL) {
}

Configuration::~Configuration() {
  delete [] m_pens;
  delete [] m_brush;
}

Configuration& Configuration::instance() {
  static Configuration obj;
  return obj;
}

namespace {
  const Qt::GlobalColor COLORS[] = {Qt::red, Qt::blue, Qt::green};
  const int COLOR_COUNT = sizeof(COLORS) / sizeof(Qt::GlobalColor);
}

void Configuration::setOwnerCount(int count) {
  if (count > COLOR_COUNT) {
    qWarning() << Q_FUNC_INFO
               << "count > COLOR_COUNT";
    return;
  }
  
  delete [] m_pens;
  delete [] m_brush;
  
  m_pens = new QPen[count];
  m_brush = new QBrush[count];
  
  for (int i = 0; i < count; ++i) {
    qDebug() << COLORS[i];
    m_pens[i].setColor(COLORS[i]);
    m_brush[i].setColor(COLORS[i]);
    m_brush[i].setStyle(Qt::SolidPattern);
  }
  
}
    
const QPen& Configuration::pointPen(int owner) const {
  return m_pens[owner - 1];
}

const QBrush& Configuration::pointBrush(int owner) const {
  return m_brush[owner - 1];
}
