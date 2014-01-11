#include <climits>
#include <queue>
#include <memory>
#include <functional>
#include <QDebug>
#include "Board.h"
#include "BorderFinder.h"
#include "IPlayerManager.h"

Board::Board(std::size_t width, std::size_t height)
    : m_cells(width, std::vector<Cell>(height))
    , m_player_manager(NULL)
{
}

Board::~Board()
{
}

QSize Board::size() const
{
    return QSize(m_cells.size(), m_cells.front().size());
}
    
void Board::setPlayerManager(IPlayerManager *manager)
{
    m_player_manager = manager;
}

const TBoardMatrix& Board::data() const
{
    return m_cells;
}

const std::list<std::pair<QPolygon, int>>& Board::polygons() const
{
    return m_polygons;
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
    {
    BorderFinder finder(m_cells, ccell.m_owner, point);
    if (!finder(&polygon))
        return false;
    }
    
    qDebug() << "Found: " << polygon;

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
                else if (!cell.isCaptured())
                {
                    has_captured = true;
                    qDebug() << "+1";
                    cell.capture();
                    
                    // TODO: Increase the score
                }
                
            }
        }
    }
    
    if (has_captured)
    {
        qDebug() << "CAPTURED";
        m_polygons.push_back(std::make_pair(polygon, ccell.m_owner));
        for (const QPoint& point : empty_points)
        {
            qDebug() << "+2";
            m_cells[point.x()][point.y()].capture();
        }
    }
    
    return true;
}