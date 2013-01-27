#pragma once
#include <QObject>

class PointCounter : public QObject {
    Q_OBJECT
    
  public:
    PointCounter(int owners, QObject *parent = 0);
    
  public slots:
    void onPointCaptured(int owner, int count);
  
  private:
    QVector<int> m_marks;
};