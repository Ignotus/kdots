#include <cassert>
#include "IPlayer.h"
#include "DotTable.h"

class IPlayer::Private
{
    friend class IPlayer;
    
private:
    IPlayer& mParent;
    
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
    Private(char player, IPlayer& parent);
    
    void pushCommand(const Command& command);
    void processCommandQueue();
    
    void init();

public:
    ~Private();
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

IPlayer::Private::Private(char player, IPlayer& parent)
    : mParent(parent)
    , mpTable(nullptr)
    , mPlayer(player)
    , mExit(true)
{
}

////////////////////////////////////////////////////////////////////////////////

IPlayer::Private::~Private()
{
    mExit = true;
    mCondition.notify_one();
    if (mThread)
        mThread->join();
}

////////////////////////////////////////////////////////////////////////////////

void IPlayer::Private::pushCommand(const Command& command)
{
    std::unique_lock<std::mutex> lock(mMutex);
    mCommandQueue.push(command);
    mMutex.unlock();
    mCondition.notify_one();
}

////////////////////////////////////////////////////////////////////////////////

void IPlayer::Private::processCommandQueue()
{
    mExit = false;
    std::unique_lock<std::mutex> lock(mMutex);
    while (!mExit)
    {
        if (mCommandQueue.empty())
            mCondition.wait(lock);
        
        if (mCommandQueue.empty() || mExit)
            return;
        
        Command poppedCommand = mCommandQueue.front();
        mCommandQueue.pop();
        
        switch (poppedCommand.mType)
        {
        case Command::OPPONENT_POINT_ADDED:
            mParent.onOpponentPointAddedImpl(poppedCommand.mX, poppedCommand.mY);
            break;
        case Command::POINT_ADDED:
            mParent.onPointAddedImpl(poppedCommand.mX, poppedCommand.mY);
            break;
        case Command::PLAYER_TURN:
            mParent.onPlayerTurnImpl();
            break;
        default:
            assert(0);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void IPlayer::Private::init()
{
    mThread.reset(new std::thread(&IPlayer::Private::processCommandQueue, this));
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

IPlayer::IPlayer(char player)
    : mP(new IPlayer::Private(player, *this))
{
}

////////////////////////////////////////////////////////////////////////////////

void IPlayer::init()
{
    mP->init();
}

////////////////////////////////////////////////////////////////////////////////

IPlayer::~IPlayer()
{
    mP.reset();
    unsetTable();
}

////////////////////////////////////////////////////////////////////////////////

void IPlayer::put(std::size_t x, std::size_t y)
{
    DotTable * const table = mP->mpTable;
    if (table == nullptr)
    {
        switch (mP->mPlayer)
        {
        case Cell::FIRST_OWNER:
            table->putFirstPlayer(x, y);
            break;
        case Cell::SECOND_OWNER:
            table->putSecondPlayer(x, y);
            break;
        default:
            assert(0);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

char IPlayer::player() const
{
    return mP->mPlayer;
}



////////////////////////////////////////////////////////////////////////////////

void IPlayer::onOpponentPointAdded(std::size_t x, std::size_t y)
{
    mP->pushCommand(Private::Command(Private::Command::OPPONENT_POINT_ADDED, x, y));
}

////////////////////////////////////////////////////////////////////////////////

void IPlayer::onPointAdded(std::size_t x, std::size_t y)
{
    mP->pushCommand(Private::Command(Private::Command::POINT_ADDED, x, y));
}

////////////////////////////////////////////////////////////////////////////////

void IPlayer::onPlayerTurn()
{
    mP->pushCommand(Private::Command(Private::Command::PLAYER_TURN));
}

////////////////////////////////////////////////////////////////////////////////

const DotTable* IPlayer::dotTable() const
{
    return mP->mpTable;
}

////////////////////////////////////////////////////////////////////////////////

void IPlayer::setTable(DotTable *table)
{
    mP->mpTable = table;
}

////////////////////////////////////////////////////////////////////////////////

void IPlayer::unsetTable()
{
    mP->mpTable = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

IPlayer::Private::Command::Command(char type)
    : mX(0)
    , mY(0)
    , mType(type)
{
}

////////////////////////////////////////////////////////////////////////////////

IPlayer::Private::Command::Command(char type, std::size_t x, std::size_t y)
    : mX(x)
    , mY(y)
    , mType(type)
{
}