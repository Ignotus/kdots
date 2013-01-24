#pragma once
#include <ostream>

class PData {
  public:
    PData();
    
    void changeToBorder();
    bool isBorder() const;
    void setOwner(int owner);
    int owner() const;
    
    void capture();
    bool isCaptured() const;
    
  private:  
    bool m_border;
    int m_owner;
    bool m_isCaptured;
};

std::ostream& operator<< (std::ostream& out, const PData& data);