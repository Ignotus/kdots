#pragma once
#include <vector>

#include <QObject>

#include "matrix.h"

struct PData {
  
};

class QPoint;
class TableModel : public QObject {
    Q_OBJECT
    
  public:
    TableModel(const QSize& size, QObject *parent = 0);
    virtual ~TableModel();
    
    const Matrix<PData>& data() const;
    
    const QSize& size() const;
 
  public slots:
    void putPoint(const QPoint& point);
    
  private:
  
  signals:
    void dataChanged();
    
  private:
    QSize m_size;
    Matrix<PData> m_data;
};