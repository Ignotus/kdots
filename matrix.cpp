#include "matrix.h"

bool isValid(const QPoint& point, const QSize& ms) {
  return (point.x() >= 0 && point.x() < ms.width()
          && point.y() >= 0 && point.y() < ms.height());
}