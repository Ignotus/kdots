#pragma once
#include <vector>
#include <list>
#include <functional>
#include <QObject>
#include <QPolygon>

#include "Cell.h"

class IPlayerManager;
class ScoreCounter;

typedef std::vector<std::vector<Cell>> TBoardMatrix;

class Board : public QObject
{
    Q_OBJECT
public:
    Board(std::size_t width, std::size_t height, QObject *parent = 0);
    ~Board();
    
    QSize size() const;
    
    void setPlayerManager(IPlayerManager *manager);
    void setScoreCounter(ScoreCounter *counter);
    
    bool put(const QPoint& point);
    
    const TBoardMatrix& data() const;
    const std::list<std::pair<QPolygon, int>>& polygons() const;

private:
    TBoardMatrix m_cells;
    std::list<std::pair<QPolygon, int>>  m_polygons;
    IPlayerManager *m_player_manager;
    ScoreCounter *m_score_counter;
    int m_processed_points;
signals:
    void gameOver();
};