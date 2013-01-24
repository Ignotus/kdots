#include "pdata.h"

PData::PData()
: m_border(false)
, m_owner(0)
, m_isCaptured(false) {
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

void PData::capture() {
  m_isCaptured = true;
}

bool PData::isCaptured() const {
  return m_isCaptured;
}

std::ostream& operator<< (std::ostream& out, const PData& data) {
  out << data.owner();
  return out;
}