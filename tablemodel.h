#pragma once
#include <memory>
#include <vector>

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
    
    int ownerCount() const;
 
  public slots:
    void putPoint(const QPoint& point);
    
  private:
    bool findCapturedBorders(const QPoint& point);
  
  signals:
    void dataChanged();
    void pointCaptured(int owner, int count);
    
  private:
    Matrix<PData> m_data;
    std::unique_ptr<OwnerDetector> m_ownerDetector;
    std::unique_ptr<PolygonFinder> m_polygonFinder;
    QPoint m_lastPoint;
};