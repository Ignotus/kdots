#pragma once
#include <cstddef>
#include <queue>
#include <condition_variable>
#include <memory>
#include <thread>

class DotTable;
class IPlayer
{
private:
    IPlayer(const IPlayer&);
    const IPlayer& operator=(const IPlayer&);
    
public:
    IPlayer(char player);
    virtual ~IPlayer();
    
    void init();
    
    void setTable(DotTable *table);
    void unsetTable();
    
    char player() const;
    
    void onOpponentPointAdded(std::size_t x, std::size_t y);
    void onPointAdded(std::size_t x, std::size_t y);
    void onPlayerTurn();
    
protected:
    virtual void onOpponentPointAddedImpl(std::size_t x, std::size_t y) = 0;
    virtual void onPointAddedImpl(std::size_t x, std::size_t y) = 0;
    virtual void onPlayerTurnImpl() = 0;
    
    const DotTable* dotTable() const;
    void put(std::size_t x, std::size_t y);
    
private:
    class Private;
    friend class Private;
    
    std::unique_ptr<Private> mP;
 
};