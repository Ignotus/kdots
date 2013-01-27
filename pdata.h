#pragma once
#include <ostream>

#include <QPoint>

#include "matrix.h"

class PData {
  public:
    PData();
    
    void changeToBorder();
    bool isBorder() const;
    void setOwner(int owner);
    int owner() const;
    
    void capture();
    bool isCaptured() const;
    
    bool hasPointTo(const QPoint& thisPoint, const QPoint& otherPoint) const;
    void addBorderTo(const QPoint& thisPoint, const QPoint& otherPoint);
    
  private:
    std::size_t hash(const QPoint& delta) const;
    
  private:  
    bool m_border;
    bool m_isCaptured;
    int m_owner;
    
    bool m_borders[DIRECTION_COUNT];
};

std::ostream& operator<< (std::ostream& out, const PData& data);