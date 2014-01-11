#pragma once
#include <vector>
#include <list> 
#include <QPolygon>
#include <functional>
#include "Cell.h"

class IPlayerManager;

typedef std::vector<std::vector<Cell>> TBoardMatrix;

class Board
{
public:
    Board(std::size_t width, std::size_t height);
    ~Board();
    
    QSize size() const;
    
    void setPlayerManager(IPlayerManager *manager);
    
    bool put(const QPoint& point);
    
    const TBoardMatrix& data() const;
    const std::list<std::pair<QPolygon, int>>& polygons() const;

private:
    TBoardMatrix m_cells;
    std::list<std::pair<QPolygon, int>>  m_polygons;
    IPlayerManager *m_player_manager;
};