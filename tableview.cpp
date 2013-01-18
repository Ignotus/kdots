#include <QMouseEvent>
#include <QPainter>

#include "tableview.h"
#include "tablemodel.h"

TableView::TableView(QWidget *parent)
: QWidget(parent)
, m_model(NULL) {
}

TableView::~TableView() {
}

void TableView::setModel(TableModel *model) {
  m_model = model;
  
  connect(model, SIGNAL(dataChanged()), this, SLOT(update()));
  connect(this, SIGNAL(pointPut(QPoint)), model, SLOT(putPoint(QPoint)));
}

void TableView::mouseReleaseEvent(QMouseEvent *e) {
  QPoint mp;
  if (modelPoint(e->pos(), mp)) {
    emit pointPut(mp);
  }
}

QPoint TableView::padding() const {
  const QSize& buffSize = pixmapSize();
  const QSize& ws = size();
  return QPoint((ws.width() - buffSize.width()) / 2,
                (ws.height() - buffSize.height()) / 2);
}

void TableView::paintEvent(QPaintEvent * /*e */) {
  const QSize& buffSize = pixmapSize();
  QPixmap buffer(buffSize);
  buffer.fill(Qt::white);
  
  QPainter painter(&buffer);
  
  drawLines(painter);
  drawDots(painter);
  drawDotsBorder(painter);
  
  QPainter wPainter(this);
  wPainter.drawPixmap(padding(), buffer);
}

//TODO: Caching this value somewhere
float TableView::squareSize() const {
  const QSize& ms = m_model->size();
  const QSize& ws = size();
  
  return std::min(float(ws.width()) / ms.width(),
                  float(ws.height()) / ms.height());
}

QSize TableView::pixmapSize() const {
  return m_model->size() * squareSize();
}

bool TableView::modelPoint(const QPoint& widgetPoint, QPoint& modelPoint) const {
  return false;
}

bool TableView::viewPoint(const QPoint& modelPoint, QPoint& viewPoint) const {
  return false;
}

void TableView::drawLines(QPainter& painter) {
  const QSize& ms = m_model->size();
  const QPaintDevice *device = painter.device();
  const int w = device->width();
  const int h = device->height();
  
  const float zero = 0.00001;
  const float sz = squareSize();
  const float shift = sz;
  for (float x = sz; x + shift - w < zero; x += shift) {
    painter.drawLine(x, 0, x, h);
  }
  
  for (float y = sz; y + shift - h < zero; y += shift) {
    painter.drawLine(0, y, w, y);
  }
  
}

void TableView::drawDots(QPainter& painter) {
  
}

void TableView::drawDotsBorder(QPainter& painter) {
}