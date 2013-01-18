#pragma once
#include <QWidget>

class QPainter;
class TableModel;
class TableView : public QWidget {
    Q_OBJECT

  public:
    TableView(QWidget *parent = 0);
    virtual ~TableView();
    
    void setModel(TableModel *model);
    
  protected:
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
    
  private:
    bool modelPoint(const QPoint& widgetPoint, QPoint& modelPoint) const;
    bool viewPoint(const QPoint& modelPoint, QPoint& viewPoint) const;
    
    void drawLines(QPainter& painter);
    void drawDots(QPainter& painter);
    void drawDotsBorder(QPainter& painter);
    QSize pixmapSize() const;
    QPoint padding() const;
    
    float squareSize() const;
  
  signals:
    void pointPut(const QPoint& point);
  
  private:
    TableModel *m_model;
};