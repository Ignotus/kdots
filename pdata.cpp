#include "pdata.h"

PData::PData()
: m_border(false)
, m_owner(0) {
}
    
void PData::changeToBorder() {
  m_border = true;
}
    
bool PData::isBorder() const {
  return m_border;
}

void PData::setOwner(int owner) {
  if (!m_owner)
    m_owner = owner;
}

int PData::owner() const {
  return m_owner;
}