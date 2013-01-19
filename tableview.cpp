#include <math.h>

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
  qDebug() << Q_FUNC_INFO;
  const QSize& buffSize = pixmapSize();
  QPixmap buffer(buffSize);
  buffer.fill(Qt::white);
  
  QPainter painter(&buffer);
  painter.setRenderHint(QPainter::Antialiasing);
  
  drawLines(painter);
  drawDots(painter);
  drawDotsBorder(painter);
  
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
  const QSize& ms = m_model->data().size();
  const QPaintDevice *device = painter.device();
  const int w = device->width();
  const int h = device->height();
  
  const float zero = 0.00001;
  const float sz = squareSize();
  qDebug() << "Check: " << sz;
  const float shift = sz;
  for (float x = sz; x + shift - w < zero; x += shift) {
    painter.drawLine(x, 0, x, h);
  }
  
  for (float y = sz; y + shift - h < zero; y += shift) {
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
        const QColor& brushColor = Configuration::instance().pointColor(point.owner());
        painter.setBrush(QBrush(brushColor));
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
      const PData& point = matrix[i][j];
      const int own = point.owner();
      if (!own || !point.isBorder()) {
        continue;
      }
      
      const QPen pen(Configuration::instance().pointColor(own));
      
      painter.setPen(pen);
      
      for (int k = 0; k < DIRECTION_COUNT; ++k) {
        const int newx = i + DX[k];
        const int newy = j + DY[k];
        
        if (!matrix[newx][newy].isBorder()) {
          continue;
        }
        
        if (newx < 0 || newy < 0 || newx >= ms.width() || newy >= ms.height()) {
          continue;
        }
        
        if (newx < i || (newx == i && newy < j)) {
          continue;
        }
        
        painter.drawLine(viewPoint(QPoint(newx, newy)), viewPoint(QPoint(i, j)));
      }
    }
  }
}