#pragma once

class PData {
  public:
    PData();
    
    void changeToBorder();
    bool isBorder() const;
    void setOwner(int owner);
    int owner() const;
    
  private:  
    bool m_border;
    int m_owner;
};