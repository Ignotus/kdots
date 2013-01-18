#pragma once

class OwnerDetector {
  public:
    OwnerDetector(int owners, int first)
    : m_owners(owners)
    , m_currentOwner(first) {
    }
    
    virtual ~OwnerDetector() {}
    
    int owner() const {
      return m_currentOwner;
    }
    
    virtual int nextOwner(bool isCaptured) = 0;
  
  protected:
    int m_owners;
    int m_currentOwner;
};

class WithoutAdditionalMove : public OwnerDetector {
  public:
    WithoutAdditionalMove(int owners, int first)
    : OwnerDetector(owners, first) {
    }
    
    int nextOwner(bool) {
        if (m_currentOwner + 1 == m_owners)
          return (m_currentOwner = 1);
        else
          return ++m_currentOwner;
      }
};

class AdditionalMove : public WithoutAdditionalMove {
  public:
    AdditionalMove(int owners, int first)
    : WithoutAdditionalMove(owners, first) {
    }
    
    int nextOwner(bool isCaptured) {
      if (isCaptured)
        return m_currentOwner;
      
      return WithoutAdditionalMove::nextOwner(isCaptured);
    }
};


