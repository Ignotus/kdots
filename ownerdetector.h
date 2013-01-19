#pragma once

class OwnerDetector {
  public:
    OwnerDetector(int owners, int first);  
    virtual ~OwnerDetector();
    int owner() const;
    
    virtual int nextOwner(bool isCaptured) = 0;
  
  protected:
    int m_owners;
    int m_currentOwner;
};

class WithoutAdditionalMove : public OwnerDetector {
  public:
    WithoutAdditionalMove(int owners, int first);
    
    int nextOwner(bool);
};

class AdditionalMove : public WithoutAdditionalMove {
  public:
    AdditionalMove(int owners, int first);
    
    int nextOwner(bool isCaptured);
};


