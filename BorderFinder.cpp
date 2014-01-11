#include <algorithm>
#include <QHash>
#include <QDebug>
#include "BorderFinder.h"

BorderFinder::BorderFinder(const TBoardMatrix& board, int current_owner, const QPoint& start_point)
    : m_board(board)
    , m_current_owner(current_owner)
    , m_width(board.size())
    , m_height(board.front().size())
    , m_start_point(start_point)
    , m_bypass_map(m_width, std::vector<bool>(m_height, false))
{
}

namespace
{
    std::size_t arrayIndex(const QPoint& point)
    {
        switch (point.y())
        {
        case -1:
            return (point.x() + point.y() + 2);
        case 0:
            if (point.x() == 1)
                return 3;
            return 7;
        }
        
        return -(point.x() + point.y());
    }
    
    bool pointCheck(const QPoint side_points[],
                    const TBoardMatrix& cells,
                    int owner,
                    const QPoint& first, const QPoint& second)
    {
        const QPoint check_point = first + side_points[arrayIndex(second - first)];
        if (check_point.x() < 0 || check_point.y() < 0 || check_point.x() >= cells.size() || check_point.y() >= cells.front().size())
            return true;

        return (cells[check_point.x()][check_point.y()].m_owner != owner);
    }
    
    long doubleSquare(const QPolygon& polygon)
    {
        long square = 0;
        for (std::size_t i = 0, max = polygon.size(); i < max; ++i)
        {
            const QPoint& first = polygon[i];
            const QPoint& second = polygon[(i + 1) % max];
            
            square += (second.y() + first.y()) * (second.x() - first.x());
        }
        
        return std::abs(square);
    }
}

bool BorderFinder::operator()(QPolygon *output)
{
    const QPoint left_side_points[] = {
        {-1, 0},
        {-1, -1},
        {0, -1},
        {1, -1},
        {1, 0},
        {1, 1},
        {0, 1},
        {-1, 1}
    };
    
    recursiveDFS(m_start_point, std::bind(pointCheck,
                                          left_side_points,
                                          m_board,
                                          m_current_owner,
                                          std::placeholders::_1,
                                          std::placeholders::_2));
    

    
    const QPoint right_side_points[] = {
        {0, -1},
        {1, -1},
        {1, 0},
        {1, 1},
        {0, 1},
        {-1, 1},
        {-1, 0},
        {-1, -1}
    };
    
    recursiveDFS(m_start_point, std::bind(pointCheck,
                                          right_side_points,
                                          m_board,
                                          m_current_owner,
                                          std::placeholders::_1,
                                          std::placeholders::_2));

    
    if (m_results.empty())
        return false;
    
    QHash<const QPolygon*, long> squares;
    for (const QPolygon& polygon : m_results)
        squares[&polygon] = doubleSquare(polygon);
    
    std::max_element(m_results.begin(), m_results.end(), [&squares] (const QPolygon& first, const QPolygon& second) {
        return squares[&first] < squares[&second];
    })->swap(*output);
    
    return true;
}

namespace
{
    const QPoint d_point[] = {
        {-1, -1},
        {0, -1},
        {1, -1},
        {1, 0},
        {1, 1},
        {0, 1},
        {-1, 1},
        {-1, 0}
    };
}

void BorderFinder::recursiveDFS(const QPoint& current_point,
                                const std::function<bool(const QPoint&, const QPoint&)>& edge_checker)
{
    m_bypass_map[current_point.x()][current_point.y()] = true;
    m_point_queue << current_point;
    
    for (std::size_t i = 0; i < 8; ++i)
    {
        const QPoint new_point = current_point + d_point[i];
        if (new_point.x() < 0 || new_point.y() < 0 || new_point.x() >= m_width || new_point.y() >= m_height)
            continue;
        
        const Cell& new_cell = m_board[new_point.x()][new_point.y()];
        
        if (new_cell.m_owner != m_current_owner || new_cell.isCaptured())
            continue;
        
        if (!edge_checker(current_point, new_point))
            continue;
        
        if (m_point_queue.size() > 3 && new_point == m_start_point)
        {
            qDebug() << "Found result: " << m_point_queue;
            m_results << m_point_queue;
            continue;
        }
        
        if (m_bypass_map[new_point.x()][new_point.y()] != true)
            recursiveDFS(new_point, edge_checker);
    }
        
    m_bypass_map[current_point.x()][current_point.y()] = false;
    m_point_queue.pop_back();
}