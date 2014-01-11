#include <climits>
#include <queue>
#include <memory>
#include <functional>
#include "Board.h"
#include "BorderFinder.h"
#include "IPlayerManager.h"
#include "ScoreCounter.h"

Board::Board(std::size_t width, std::size_t height, QObject *parent)
    : QObject(parent)
    , m_cells(width, std::vector<Cell>(height))
    , m_player_manager(NULL)
    , m_score_counter(NULL)
    , m_processed_points(0)
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

void Board::setScoreCounter(ScoreCounter *counter)
{
    m_score_counter = counter;
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
    
    ++m_processed_points;
    ccell.m_owner = m_player_manager->currentPlayer();
    bool isCaptured = false;
    
    std::shared_ptr<void> next_player_handler(nullptr, [&](void*) {
        m_player_manager->nextPlayer(isCaptured);
    });
    
    
    QPolygon polygon;
    {
    BorderFinder finder(m_cells, ccell.m_owner, point);
    if (!finder(&polygon))
    {
        if (m_processed_points == width * height)
            emit gameOver();
        return false;
    }
    }
    
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
                    cell.capture();
                    
                    m_score_counter->increase(ccell.m_owner);
                }
                
            }
        }
    }
    
    if (has_captured)
    {
        m_polygons.push_back(std::make_pair(polygon, ccell.m_owner));
        for (const QPoint& point : empty_points)
        {
            m_cells[point.x()][point.y()].capture();
        }
    }
    
    if (m_processed_points == width * height)
        emit gameOver();

    return true;
}