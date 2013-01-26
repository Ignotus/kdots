#pragma once
#include <QColor>

class QPen;
class QBrush;
class Configuration {
  public:
    static Configuration& instance();
    
    const QPen& pointPen(int owner) const;
    const QBrush& pointBrush(int owner) const;
    void setOwnerCount(int count);
    
  private:
    Configuration();
    Configuration(const Configuration& other);
    ~Configuration();
    
  private:
    QPen *m_pens;
    QBrush *m_brush;
    
};