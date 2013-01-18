#pragma once
#include <vector>

#include <QSize>
#include <QPoint>

class QSize;

template<class T>
class Matrix {
  public:
    typedef std::vector<T> MatrixLineType;
    typedef std::vector<MatrixLineType> MatrixDataType;
    
    Matrix(const QSize& size)
    : m_data(MatrixDataType(size.width(), MatrixLineType(size.height()))) {
    }
    
    T& operator[](const QPoint& point) {
      return m_data[point.x()][point.y()];
    }
    
    const T& operator[](const QPoint& point) const {
      return m_data[point.x()][point.y()];
    }
    
    MatrixLineType& operator[](int x) {
      return m_data[x];
    }
    
    const MatrixLineType& operator[](int x) const {
      return m_data[x];
    }
  private:
    MatrixDataType m_data;
};