#pragma once
#include <cstddef>

class DotTable;
class IPlayer
{
    DotTable *mpTable;
    const char mPlayer;
public:
    IPlayer(char player);
    virtual ~IPlayer();
    
    void registerTable(DotTable *table);
    void unregisterTable();
    
    char player() const;
    
    virtual void onOpponentPointAdded(std::size_t x, std::size_t y) = 0;
    virtual void onPointAdded(std::size_t x, std::size_t y) = 0;
    virtual void onPlayerTurn() = 0;
    
protected:
    const DotTable* dotTable() const;
    void put(std::size_t x, std::size_t y);
};