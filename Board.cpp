#include <climits>
#include <queue>
#include <memory>
#include <functional>
#include "Board.h"
#include "IPlayerManager.h"

Board::Board(std::size_t width, std::size_t height)
    : m_cells(width, std::vector<Cell>(height))
    , m_player_manager(NULL)
{
}

Board::~Board()
{
}
    
void Board::setPlayerManager(IPlayerManager *manager)
{
    m_player_manager = manager;
}

namespace
{
    std::size_t arrayIndex(const QPoint& point)
    {
        if (point.y() == -1 || (point.y() == 0 && point.x() == -1))
            return (point.x() + 1 + (point.y() + 1) * 3);
        
        return (point.x() + (point.y() + 1) * 3);
    }
    
    bool pointCheck(const QPoint side_points[],
                    const std::vector<std::vector<Cell>>& cells,
                    int owner,
                    const QPoint& first, const QPoint& second)
    {
        const QPoint check_point = first + side_points[arrayIndex(second - first)];
        return (cells[check_point.x()][check_point.y()].m_owner != owner);
    }
}

bool Board::getMaxPolygon(QPolygon *polygon,
                          const QPoint& begin_point) const
{
    const int owner = m_player_manager->currentPlayer();

    long left_square, right_square;
    QPolygon left_poly, right_poly;
    
    const QPoint left_side_points[] = {
        {-1, 0},
        {-1, 1},
        {0, 1},
        {1, 1},
        {1, 0},
        {1, -1},
        {0, -1},
        {-1, -1}
    };
    
    getMaxPolygon(&left_poly, &left_square, begin_point, std::bind(pointCheck,
                                                                   left_side_points,
                                                                   m_cells,
                                                                   owner,
                                                                   std::placeholders::_1,
                                                                   std::placeholders::_2));
    
    const QPoint right_side_points[] = {
        {0, 1},
        {1, 1},
        {1, 0},
        {1, -1},
        {0, -1},
        {-1, -1},
        {-1, 0},
        {-1, 1}
    };
    
    getMaxPolygon(&right_poly, &right_square, begin_point, std::bind(pointCheck,
                                                                   right_side_points,
                                                                   m_cells,
                                                                   owner,
                                                                   std::placeholders::_1,
                                                                   std::placeholders::_2));
    
    if (left_square == 0 && right_square == 0)
        return false;
    
    if (left_square > right_square)
        polygon->swap(left_poly);
    else
        polygon->swap(right_poly);
    
    return true;
}

namespace
{
    long doubleSquare(const QPolygon& polygon)
    {
        long square = 0;
        for (std::size_t i = 0, max = polygon.size(); i < max; ++i)
        {
            const QPoint& first = polygon[i];
            const QPoint& second = polygon[(i + 1) % max];
            
            square += (second.y() + first.y()) * (second.x() - first.x());
        }
        
        return square;
    }
}

bool Board::getMaxPolygon(QPolygon *polygon,
                          long *square,
                          const QPoint& begin_point,
                          std::function<bool(const QPoint&, const QPoint&)> edge_checker) const
{
    const int width = m_cells.size();
    const int height = m_cells.front().size();
    const int owner = m_player_manager->currentPlayer();
    
    const int dx[] = {-1,  0,  1, 1, 1, 0, -1, -1};
    const int dy[] = {-1, -1, -1, 0, 1, 1,  1,  0};
    
    std::vector<std::vector<bool>> flag_table(width, std::vector<bool>(height, false));
    
    std::list<QPoint> point_queue;
    point_queue.push_back(begin_point);
    
    bool status = false;
    
    long max_square = 0;
    QPolygon result;
    
    while (!point_queue.empty())
    {
        const QPoint& c_point = point_queue.front();
        flag_table[c_point.x()][c_point.y()] = true;
        
        for (std::size_t i = 0; i < 8; ++i)
        {
            const QPoint new_point = c_point + QPoint(dx[i], dy[i]);
            if (new_point.x() < 0 || new_point.y() < 0 || new_point.x() >= width
                    || new_point.y() >= height)
                continue;
            
            if (m_cells[new_point.x()][new_point.y()].m_owner != owner
                    || flag_table[new_point.x()][new_point.y()] == true)
                continue;
            
            if (!edge_checker(c_point, new_point))
                continue;
            
            point_queue.push_back(new_point);
            if (std::abs(new_point.x() - begin_point.x()) < 2
                    && std::abs(new_point.y() - begin_point.y()) < 2)
            {
                status = true;
                QPolygon poly = QPolygon::fromList(QList<QPoint>::fromStdList(point_queue));
                
                const long poly_square = doubleSquare(poly);
                if (poly_square > max_square)
                {
                    max_square = poly_square;
                    result.swap(poly);
                }
            }
        }
        
        flag_table[c_point.x()][c_point.y()] = false;
        
        point_queue.pop_back();
    }
    
    if (status)
    {
        polygon->swap(result);
        *square = max_square;
    }
    else
    {
        *square = 0;
    }
    
    return status;
}

bool Board::put(const QPoint& point)
{
    const std::size_t width = m_cells.size();
    const std::size_t height = m_cells.front().size();
    if (static_cast<std::size_t>(point.x()) >= width || static_cast<std::size_t>(point.y()) >= height)
        return false;
    
    Cell& ccell = m_cells[point.x()][point.y()];
    
    if (ccell.m_owner != -1)
        return false;
    
    ccell.m_owner = m_player_manager->currentPlayer();
    bool isCaptured = false;
    
    std::shared_ptr<void> next_player_handler(nullptr, [&](void*) {
        m_player_manager->nextPlayer(isCaptured);
    });
    
    QPolygon polygon;
    if (!getMaxPolygon(&polygon, point))
        return true;
    

    std::list<QPoint> empty_points;
    bool has_captured = false;
    for (std::size_t i = 0; i < width; ++i)
    {
        for (std::size_t j = 0; j < height; ++j)
        {
            Cell& cell = m_cells[i][j];
            if (cell.m_owner == ccell.m_owner)
                continue;
            
            const QPoint c_point(i, j);
            
            if (polygon.containsPoint(c_point, Qt::OddEvenFill))
            {
                if (cell.m_owner == -1)
                    empty_points.push_back(c_point);
                else
                {
                    has_captured = true;
                    cell.capture();
                    
                    // TODO: Increase the score
                }
                
            }
        }
    }
    
    if (has_captured)
    {
        for (const QPoint& point : empty_points)
            m_cells[point.x()][point.y()].capture();
    }
    
    return true;
}