#pragma once
#include <cstddef>
#include <queue>
#include <condition_variable>
#include <memory>
#include <thread>

class DotTable;
class IPlayer
{
public:
    IPlayer(char player);
    virtual ~IPlayer();
    
    void init();
    
    void setTable(DotTable *table);
    void unsetTable();
    
    char player() const;
    
    //TODO: Command Sequence
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
    DotTable *mpTable;
    const char mPlayer;
    
    std::unique_ptr<std::thread> mThread;
    std::mutex mMutex;
    std::condition_variable mCondition;
    volatile bool mExit;
    
    struct Command
    {
    public:
        enum Type
        {
            NONE = 0,
            POINT_ADDED,
            OPPONENT_POINT_ADDED,
            PLAYER_TURN
        };
        
        std::size_t mX;
        std::size_t mY;
        char mType;
        
        Command(char type);
        Command(char type, std::size_t x, std::size_t y);
    };
    
    std::queue<Command> mCommandQueue;

private:
    void pushCommand(const Command& command);
    void processCommandQueue();
};