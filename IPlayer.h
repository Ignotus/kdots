#pragma once
#include <cstddef>

class DotTable;
class IPlayer
{
public:
    IPlayer(char player);
    virtual ~IPlayer();
    
    void setTable(DotTable *table);
    void unsetTable();
    
    char player() const;
    
    virtual void onOpponentPointAdded(std::size_t x, std::size_t y) = 0;
    virtual void onPointAdded(std::size_t x, std::size_t y) = 0;
    virtual void onPlayerTurn() = 0;
    
protected:
    const DotTable* dotTable() const;
    void put(std::size_t x, std::size_t y);
    
private:
    DotTable *mpTable;
    const char mPlayer;
};