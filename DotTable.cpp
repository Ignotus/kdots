#include <cassert>
#include <thread>
#include "DotTable.h"
#include "IPlayingStrategy.h"
#include "IPlayer.h"
#include "WithExtraMovePlayingStrategy.h"

class DotTable::Private
{
private:
    friend class DotTable;
    
    Private(std::size_t width, std::size_t height, char firstOwner);
    char put(std::size_t x, std::size_t y);
    bool checkedOwnerPut(std::size_t x, std::size_t y, char owner);
    void notifyPlayers(std::size_t x, std::size_t y, char owner, char nextTurnOwner);
    void notifyPlayerImpl(IPlayer *player, std::size_t x, std::size_t y, char owner);
    
    Private(const Private&);
    const Private& operator=(const Private&);    

private:
    const std::size_t mWidth;
    const std::size_t mHeight;
    const char mFirstPlayer;
    std::vector< std::vector< Cell > > mData;
    std::unique_ptr< IPlayingStrategy > mPlayingStrategy;
    
    UndoStack *mpUndoStack;
    IPlayer *mpFirstPlayer;
    IPlayer *mpSecondPlayer;
};

////////////////////////////////////////////////////////////////////////////////

DotTable::Private::Private(std::size_t width, std::size_t height, char firstOwner)
    : mWidth(width)
    , mHeight(height)
    , mFirstPlayer(firstOwner)
    , mData(mWidth, std::vector< Cell >(mHeight))
    , mpUndoStack(nullptr)
    , mpFirstPlayer(nullptr)
    , mpSecondPlayer(nullptr)
{
}

////////////////////////////////////////////////////////////////////////////////

char DotTable::Private::put(std::size_t x, std::size_t y)
{
    const char owner = mPlayingStrategy->currentPlayer();
    
    mData[x][y].setOwner(owner);
    
    
    const bool isCaptured = false;
    return mPlayingStrategy->nextTurn(isCaptured);
}

////////////////////////////////////////////////////////////////////////////////

bool DotTable::Private::checkedOwnerPut(std::size_t x, std::size_t y, char owner)
{
    if (mPlayingStrategy->currentPlayer() != owner)
        return false;
    
    const Cell& currentCell = mData[x][y];
    if (currentCell.hasOwner())
        return false;
    
    const char nextTurnOwner = put(x, y);
    
    return true;
}

////////////////////////////////////////////////////////////////////////////////

void DotTable::Private::notifyPlayerImpl(IPlayer *player, std::size_t x,
                                         std::size_t y, char owner)
{
    player->onPointAdded(x, y);
    if (player->player() != owner)
        player->onOpponentPointAdded(x, y);
}

////////////////////////////////////////////////////////////////////////////////

void DotTable::Private::notifyPlayers(std::size_t x, std::size_t y, char owner,
                                      char nextTurnOwner)
{
    if (mpFirstPlayer == nullptr || mpSecondPlayer == nullptr)
        return;
    
    notifyPlayerImpl(mpFirstPlayer, x, y, owner);
    notifyPlayerImpl(mpSecondPlayer, x, y, owner);
    
    if (mpFirstPlayer->player() == nextTurnOwner)
        mpFirstPlayer->onPlayerTurn();
    else
        mpSecondPlayer->onPlayerTurn();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

DotTable::DotTable(std::size_t width, std::size_t height, char firstOwner)
    : mP(new Private(width, height, firstOwner))
{
    setPlayingStrategy(std::unique_ptr< IPlayingStrategy >(new WithExtraMovePlayingStrategy()));
}

////////////////////////////////////////////////////////////////////////////////

DotTable::~DotTable()
{
}

////////////////////////////////////////////////////////////////////////////////

void DotTable::registerPlayer(IPlayer *player)
{
    switch (player->player())
    {
    case Cell::FIRST_OWNER:
        mP->mpFirstPlayer = player;
        break;
    case Cell::SECOND_OWNER:
        mP->mpSecondPlayer = player;
        break;
    default:
        assert(0);
    }
    
    player->setTable(this);
}

////////////////////////////////////////////////////////////////////////////////

void DotTable::unregisterPlayer(IPlayer *player)
{
    switch (player->player())
    {
    case Cell::FIRST_OWNER:
        mP->mpFirstPlayer = nullptr;
        break;
    case Cell::SECOND_OWNER:
        mP->mpSecondPlayer = nullptr;
        break;
    default:
        assert(0);
    }
    
    player->unsetTable();
}

////////////////////////////////////////////////////////////////////////////////

std::size_t DotTable::width() const
{
    return mP->mWidth;
}

////////////////////////////////////////////////////////////////////////////////

std::size_t DotTable::height() const
{
    return mP->mHeight;
}

////////////////////////////////////////////////////////////////////////////////

void DotTable::setPlayingStrategy(std::unique_ptr< IPlayingStrategy > strategy)
{
    std::unique_ptr< IPlayingStrategy >& playingStrategy = mP->mPlayingStrategy;
    strategy.swap(playingStrategy);
    playingStrategy->init(mP->mFirstPlayer);
}

////////////////////////////////////////////////////////////////////////////////

const Cell* DotTable::cell(std::size_t x, std::size_t y) const
{
    if (x < width() && y < height())
        return &mP->mData[x][y];
    
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////

bool DotTable::putFirstPlayer(std::size_t x, std::size_t y)
{
    return mP->checkedOwnerPut(x, y, Cell::FIRST_OWNER);
}

////////////////////////////////////////////////////////////////////////////////

bool DotTable::putSecondPlayer(std::size_t x, std::size_t y)
{
    return mP->checkedOwnerPut(x, y, Cell::SECOND_OWNER);
}

////////////////////////////////////////////////////////////////////////////////

void DotTable::registerUndoManager(UndoStack *stack)
{
    mP->mpUndoStack = stack;
}

////////////////////////////////////////////////////////////////////////////////

bool DotTable::operator==(const DotTable& other) const
{
    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool DotTable::operator!=(const DotTable& other) const
{
    return true;
}

////////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& out, const DotTable& table)
{
    return out;
}

////////////////////////////////////////////////////////////////////////////////

std::istream& operator>>(std::istream& in, DotTable& table)
{
    return in;
}
