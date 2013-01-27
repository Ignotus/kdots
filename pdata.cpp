#include "pdata.h"

PData::PData()
: m_border(false)
, m_isCaptured(false) 
, m_owner(0)
, m_borders({0, 0, 0, 0, 0, 0, 0, 0}) {
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

std::size_t PData::hash(const QPoint& delta) const {
  const std::size_t hash = 3 * delta.x() + delta.y() + 4;
  return hash - (hash > 4);
}

bool PData::hasPointTo(const QPoint& thisPoint, const QPoint& otherPoint) const {
  return m_borders[hash(otherPoint - thisPoint)];
}

void PData::addBorderTo(const QPoint& thisPoint, const QPoint& otherPoint) {
  m_borders[hash(otherPoint - thisPoint)] = true;
}

std::ostream& operator<< (std::ostream& out, const PData& data) {
  out << data.owner();
  return out;
}