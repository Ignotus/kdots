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
  
  connect(model, SIGNAL(dataChanged()), this, SLOT(updateView()));
  connect(this, SIGNAL(pointPut(QPoint)), model, SLOT(putPoint(QPoint)));
}

void TableView::mouseReleaseEvent(QMouseEvent *e) {
  const QPoint& mp = modelPoint(e->pos());
  emit pointPut(mp);
}

void TableView::updateView() {
  QPixmap buffer(m_model->size());
  buffer.fill(Qt::white);
  
  QPainter painter(&buffer);
  
  drawLines(painter);
  drawDots(painter);
  drawDotsBorder(painter);
}

QPoint TableView::modelPoint(const QPoint& widgetPoint) const {
  return QPoint();
}

void TableView::drawLines(QPainter& painter) {
}

void TableView::drawDots(QPainter& painter) {
}

void TableView::drawDotsBorder(QPainter& painter) {
}