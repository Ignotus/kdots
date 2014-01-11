#pragma once
#include <memory>
#include <functional>
#include "Board.h"

class BorderFinder
{
public:
    BorderFinder(const TBoardMatrix& board, int current_owner, const QPoint& start_point);
    
    bool operator()(QPolygon *output);
    
private:
    void recursiveDFS(const QPoint& current_point,
                      const std::function<bool(const QPoint&, const QPoint&)>& edge_checker);
    
private:
    const TBoardMatrix& m_board;
    const int m_current_owner;
    const int m_width;
    const int m_height;
    const QPoint& m_start_point;
    QPolygon m_result;
    std::vector<std::vector<bool>> m_bypass_map;
    QPolygon m_point_queue;
    QList<QPolygon> m_results;
};