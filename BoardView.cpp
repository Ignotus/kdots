#include <cmath>
#include <iostream>
#include <complex>
#include <complex>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include "Board.h"
#include "BoardView.h"

BoardView::BoardView(QWidget *parent)
    : QWidget(parent)
    , m_model(nullptr)
{
}

BoardView::~BoardView()
{
}
    
void BoardView::setModel(Board *board)
{
    m_model = board;
}

void BoardView::mouseReleaseEvent(QMouseEvent *e)
{
    QPoint mp;
    if (modelPoint(e->pos(), &mp))
    {
        m_model->put(mp);
        
        update();
    }
}

bool BoardView::modelPoint(const QPoint& widgetPoint, QPoint *mp) const
{
  const QPoint& ps = padding();
  const float sz = squareSize();
  
  const QPoint& dp = widgetPoint - ps - QPoint(sz, sz);
  const float xc = dp.x() / sz;
  const float yc = dp.y() / sz;
  
  mp->setX(round(xc));
  mp->setY(round(yc));
  
  const QSize& ms = m_model->size();
  if (mp->x() < 0 || mp->y() < 0 || mp->x() >= ms.width() || mp->y() >= ms.height())
    return false;
  
  const float r2 = pow(mp->x() - xc, 2) + pow(mp->y() - yc, 2);
  
  return r2 < 0.3 * 0.3;
}

QSize BoardView::pixmapSize() const
{
  return (m_model->size() + QSize(1, 1)) * squareSize();
}

QPoint BoardView::padding() const
{
  const QSize& buffSize = pixmapSize();
  const QSize& ws = size();
  return QPoint((ws.width() - buffSize.width()) / 2,
                (ws.height() - buffSize.height()) / 2);
}

void BoardView::paintEvent(QPaintEvent *e)
{
    const QSize& buffSize = pixmapSize();
    QPixmap buffer(buffSize);
    buffer.fill(Qt::white);
    
    QPainter painter(&buffer);
    painter.setRenderHint(QPainter::Antialiasing);
    
    drawGrid(painter);
    if (m_model)
    {
        drawDots(painter);
        drawBorders(painter);
    }
    
    QPainter wPainter(this);
    wPainter.drawPixmap(padding(), buffer);
}

float BoardView::squareSize() const
{
  const QSize& ms = m_model->size();
  const QSize& ws = size();
  
  return std::min(float(ws.width()) / (ms.width() + 1),
                  float(ws.height()) / (ms.height() + 1));
}

void BoardView::drawGrid(QPainter& painter)
{
    const QPaintDevice *device = painter.device();
    const int w = device->width();
    const int h = device->height();
    
    const double zero = 0.001;
    const float sz = squareSize();
    
    const float shift = sz;
    for (float x = sz; w - x > zero; x += shift)
    {
        painter.drawLine(x, 0, x, h);
    }
    
    for (float y = sz; h - y > zero; y += shift)
    {
        painter.drawLine(0, y, w, y);
    }
}

QPoint BoardView::viewPoint(const QPoint& modelPoint) const
{
  return (modelPoint + QPoint(1, 1)) * squareSize();
}

void BoardView::drawDots(QPainter& painter)
{
  const std::vector<std::vector<Cell>>& matrix = m_model->data();
  const QSize& ms = m_model->size();
  
  for (int i = 0, xmax = ms.width(); i < xmax; ++i)
  {
    for (int j = 0, ymax = ms.height(); j < ymax; ++j)
    {
      const Cell& point = matrix[i][j];
      if (point.m_owner != -1)
      {
        const QBrush brush = point.m_owner == 0 ? QBrush(Qt::red) : QBrush(Qt::blue);
        painter.setBrush(brush);
        painter.drawEllipse(viewPoint(QPoint(i, j)), 5, 5);
      }
    }
  }
}

void BoardView::drawBorders(QPainter& painter)
{
    const std::list<std::pair<QPolygon, int>>& polygons = m_model->polygons();
    for (const std::pair<QPolygon, int>& polygon : polygons)
    {
        const QPen pen = polygon.second == 0 ? QPen(Qt::red, 2) : QPen(Qt::blue, 2);
        painter.setPen(pen);
        
        QPolygon real_points;
        for (const QPoint& point : polygon.first)
        {
            real_points.push_back(viewPoint(point));
        }
        
        real_points.push_back(real_points.front());
        
        painter.drawPolyline(real_points);
    }
}