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
    
  private:
    QPoint modelPoint(const QPoint& widgetPoint) const;
    void drawLines(QPainter& painter);
    void drawDots(QPainter& painter);
    void drawDotsBorder(QPainter& painter);
  
  public slots:
    void updateView();
    
  signals:
    void pointPut(const QPoint& point);
  
  private:
    TableModel *m_model;
};