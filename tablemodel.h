#pragma once
#include <vector>

#include <QDebug>
#include <QObject>

#include "matrix.h"
#include "pdata.h"

class QPoint;
class OwnerDetector;
class PolygonFinder;
class TableModel : public QObject {
    Q_OBJECT
    
  public:
    TableModel(const QSize& size, QObject *parent = 0);
    virtual ~TableModel();
    
    void setOwnerDetector(OwnerDetector *detector);
    void setPolygonFinder(PolygonFinder *finder);
    
    const Matrix<PData>& data() const;
    
    const QPoint& lastPoint() const;
 
  public slots:
    void putPoint(const QPoint& point);
    
  private:
    bool findCapturedBorders(const QPoint& point);
  
  signals:
    void dataChanged();
    
  private:
    Matrix<PData> m_data;
    OwnerDetector *m_ownerDetector;
    PolygonFinder *m_polygonFinder;
    QPoint m_lastPoint;
};