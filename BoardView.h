#pragma once
#include <QWidget>

class Board;

class BoardView : public QWidget
{
    Q_OBJECT
public:
    BoardView(QWidget *parent = 0);
    virtual ~BoardView();
    
    void setModel(Board *board);
    
protected:
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);

private:
    QSize pixmapSize() const;
    QPoint padding() const;
    float squareSize() const;
    QPoint viewPoint(const QPoint& modelPoint) const;
    bool modelPoint(const QPoint& widgetPoint, QPoint *mp) const;
    
    void drawGrid(QPainter& painter);
    void drawDots(QPainter& painter);
    void drawBorders(QPainter& painter);
    
private:
    Board *m_model;
};