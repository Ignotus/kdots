#include <cassert>
#include "IPlayer.h"
#include "DotTable.h"

IPlayer::IPlayer(char player)
    : mpTable(nullptr)
    , mPlayer(player)
    , mExit(true)
{
}

////////////////////////////////////////////////////////////////////////////////

void IPlayer::init()
{
    mThread.reset(new std::thread(&IPlayer::processCommandQueue, this));
}

////////////////////////////////////////////////////////////////////////////////

IPlayer::~IPlayer()
{
    mExit = true;
    mCondition.notify_one();
    if (mThread)
        mThread->join();
    
    unsetTable();
}

////////////////////////////////////////////////////////////////////////////////

void IPlayer::put(std::size_t x, std::size_t y)
{
    if (mpTable == nullptr)
    {
        switch (mPlayer)
        {
        case Cell::FIRST_OWNER:
            mpTable->putFirstPlayer(x, y);
            break;
        case Cell::SECOND_OWNER:
            mpTable->putSecondPlayer(x, y);
            break;
        default:
            assert(0);
        }
            
    }
}

////////////////////////////////////////////////////////////////////////////////

char IPlayer::player() const
{
    return mPlayer;
}

////////////////////////////////////////////////////////////////////////////////

void IPlayer::pushCommand(const Command& command)
{
    std::unique_lock<std::mutex> lock(mMutex);
    mCommandQueue.push(command);
    mMutex.unlock();
    mCondition.notify_one();
}

////////////////////////////////////////////////////////////////////////////////

void IPlayer::processCommandQueue()
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
            onOpponentPointAddedImpl(poppedCommand.mX, poppedCommand.mY);
            break;
        case Command::POINT_ADDED:
            onPointAddedImpl(poppedCommand.mX, poppedCommand.mY);
            break;
        case Command::PLAYER_TURN:
            onPlayerTurnImpl();
            break;
        default:
            assert(0);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void IPlayer::onOpponentPointAdded(std::size_t x, std::size_t y)
{
    pushCommand(Command(Command::OPPONENT_POINT_ADDED, x, y));
}

////////////////////////////////////////////////////////////////////////////////

void IPlayer::onPointAdded(std::size_t x, std::size_t y)
{
    pushCommand(Command(Command::POINT_ADDED, x, y));
}

////////////////////////////////////////////////////////////////////////////////

void IPlayer::onPlayerTurn()
{
    pushCommand(Command(Command::PLAYER_TURN));
}

////////////////////////////////////////////////////////////////////////////////

const DotTable* IPlayer::dotTable() const
{
    return mpTable;
}

////////////////////////////////////////////////////////////////////////////////

void IPlayer::setTable(DotTable *table)
{
    mpTable = table;
}

////////////////////////////////////////////////////////////////////////////////

void IPlayer::unsetTable()
{
    mpTable = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

IPlayer::Command::Command(char type)
    : mX(0)
    , mY(0)
    , mType(type)
{
}

////////////////////////////////////////////////////////////////////////////////

IPlayer::Command::Command(char type, std::size_t x, std::size_t y)
    : mX(x)
    , mY(y)
    , mType(type)
{
}