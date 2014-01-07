#pragma once

#include <vector>
#include <QPolygon>
#include <functional>
#include "Cell.h"

class IPlayerManager;

class Board
{
public:
    Board(std::size_t width, std::size_t height);
    ~Board();
    
    void setPlayerManager(IPlayerManager *manager);
    
    bool put(const QPoint& point);

private:
    bool getMaxPolygon(QPolygon *polygon,
                       const QPoint& begin_point) const;

    bool getMaxPolygon(QPolygon *polygon,
                       long *square,
                       const QPoint& begin_point,
                       std::function<bool(const QPoint&, const QPoint&)> edge_checker) const;
                       
private:
    std::vector<std::vector<Cell>> m_cells;
    IPlayerManager *m_player_manager;
};