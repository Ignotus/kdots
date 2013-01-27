#include <cmath>

#include <QMouseEvent>
#include <QPainter>

#include "tableview.h"
#include "tablemodel.h"
#include "configuration.h"

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
  
  Configuration::instance().setOwnerCount(m_model->ownerCount());
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
  painter.setRenderHint(QPainter::Antialiasing);
  
  drawLines(painter);
  if (m_model) {
    drawDots(painter);
    drawDotsBorder(painter);
  }
  
  QPainter wPainter(this);
  wPainter.drawPixmap(padding(), buffer);
}

//TODO: Caching this value somewhere
float TableView::squareSize() const {
  const QSize& ms = m_model->data().size();
  const QSize& ws = size();
  
  return std::min(float(ws.width()) / (ms.width() + 1),
                  float(ws.height()) / (ms.height() + 1));
}

QSize TableView::pixmapSize() const {
  return (m_model->data().size() + QSize(1, 1)) * squareSize();
}

bool TableView::modelPoint(const QPoint& widgetPoint, QPoint& mp) const {
  const QPoint& ps = padding();
  const float sz = squareSize();
  
  const QPoint& dp = widgetPoint - ps - QPoint(sz, sz);
  const float xc = dp.x() / sz;
  const float yc = dp.y() / sz;
  
  mp.setX(round(xc));
  mp.setY(round(yc));
  
  const QSize& ms = m_model->data().size();
  if (mp.x() < 0 || mp.y() < 0 || mp.x() >= ms.width() || mp.y() >= ms.height())
    return false;
  
  const float r2 = pow(mp.x() - xc, 2) + pow(mp.y() - yc, 2); 
  
  return r2 < 0.3 * 0.3;
}

QPoint TableView::viewPoint(const QPoint& modelPoint) const {
  return (modelPoint + QPoint(1, 1)) * squareSize();
}

void TableView::drawLines(QPainter& painter) {
  const QPaintDevice *device = painter.device();
  const int w = device->width();
  const int h = device->height();
  
  const double zero = 0.001;
  const float sz = squareSize();
  
  const float shift = sz;
  for (float x = sz; w - x > zero; x += shift) {
    painter.drawLine(x, 0, x, h);
  }
  
  for (float y = sz; h - y > zero; y += shift) {
    painter.drawLine(0, y, w, y);
  }
}

void TableView::drawDots(QPainter& painter) {
  typedef Matrix<PData> MatrixType;
  const MatrixType& matrix = m_model->data();
  const QSize& ms = m_model->data().size();
  
  for (int i = 0, xmax = ms.width(); i < xmax; ++i) {
    for (int j = 0, ymax = ms.height(); j < ymax; ++j) {
      const PData& point = matrix[i][j];
      if (point.owner()) {
        const QBrush& brush = Configuration::instance().pointBrush(point.owner());
        painter.setBrush(brush);
        painter.drawEllipse(viewPoint(QPoint(i, j)), 5, 5);
      }
    }
  }
}

void TableView::drawDotsBorder(QPainter& painter) {
  typedef Matrix<PData> MatrixType;
  const MatrixType& matrix = m_model->data();
  const QSize& ms = m_model->data().size();
  
  for (int i = 0, xmax = ms.width(); i < xmax; ++i) {
    for (int j = 0, ymax = ms.height(); j < ymax; ++j) {
      const QPoint point(i, j);
      const PData& pd = matrix[point];
      const int own = pd.owner();
      if (!own || !pd.isBorder()) {
        continue;
      }
      
      painter.setPen(Configuration::instance().pointPen(own));
      
      for (std::size_t k = 0; k < DIRECTION_COUNT; ++k) {
        const QPoint newPoint(i + DX[k], j + DY[k]);
        
        const PData& pdata = matrix[newPoint];
        
        if (!isValid(newPoint, ms) || pdata.owner() != own || !pdata.isBorder()) {
          continue;
        }
        
        if (newPoint.x() < i || (newPoint.x() == i && newPoint.y() < j)) {
          continue;
        }
        
        if (!pd.hasPointTo(point, newPoint)) {
          continue;
        }
        
        painter.drawLine(viewPoint(newPoint), viewPoint(point));
      }
    }
  }
}