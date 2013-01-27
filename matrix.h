#pragma once
#include <vector>
#include <iostream>

#include <QSize>
#include <QPoint>
#include <QPolygon>

class QSize;

const std::size_t DIRECTION_COUNT = 8;

const int DX[] = {-1,  0,  1,  1,  1,  0, -1, -1};
const int DY[] = {-1, -1, -1,  0,  1,  1,  1,  0};

template<class T>
class Matrix {
  public:
    typedef std::vector<T> MatrixLineType;
    typedef std::vector<MatrixLineType> MatrixDataType;
    
    Matrix(const QSize& size)
    : m_data(MatrixDataType(size.width(), MatrixLineType(size.height())))
    , m_size(size) {
    }
    
    Matrix(const QSize& size, const T& initValue)
    : m_data(MatrixDataType(size.width(), MatrixLineType(size.height(), initValue)))
    , m_size(size) {
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
    
    const QSize& size() const {
      return m_size;
    }
    
    void print() const {
      std::cout << "===============================" << std::endl;
      for (int i = 0; i < m_size.height(); ++i) {
        for (int j = 0; j < m_size.width(); ++j) {
          std::cout << (int)m_data[j][i];
        }
        
        std::cout << std::endl;
      }
    }
    
  private:
    MatrixDataType m_data;
    QSize m_size;
};

bool isValid(const QPoint& point, const QSize& ms);
QPoint next(const QPolygon::const_iterator& it,const QPolygon& poly);