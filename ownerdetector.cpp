#include "ownerdetector.h"

OwnerDetector::OwnerDetector(int owners, int first)
: m_owners(owners)
, m_currentOwner(first) {
}
    
OwnerDetector::~OwnerDetector() {
}
    
int OwnerDetector::owner() const {
  return m_currentOwner;
}

int OwnerDetector::owners() const {
  return m_owners;
}

WithoutAdditionalMove::WithoutAdditionalMove(int owners, int first)
: OwnerDetector(owners, first) {
}
    
int WithoutAdditionalMove::nextOwner(bool) {
  if (m_currentOwner == m_owners)
    return (m_currentOwner = 1);
  else
    return ++m_currentOwner;
}

AdditionalMove::AdditionalMove(int owners, int first)
: WithoutAdditionalMove(owners, first) {
}

int AdditionalMove::nextOwner(bool isCaptured) {
  if (isCaptured)
    return m_currentOwner;

  return WithoutAdditionalMove::nextOwner(isCaptured);
}

