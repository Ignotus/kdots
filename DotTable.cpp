#include <cassert>
#include "DotTable.h"
#include "IPlayingStrategy.h"
#include "IPlayer.h"

class DotTable::Private
{
private:
    friend class DotTable;
    
    Private(std::size_t width, std::size_t height, char firstOwner);
    
    Private(const Private&);
    const Private& operator=(const Private&);    

private:
    const std::size_t mWidth;
    const std::size_t mHeight;
    const char mFirstPlayer;
    std::vector< std::vector< Cell > > mData;
    std::unique_ptr<IPlayingStrategy> mPlayingStategy;
    
    UndoStack *mpUndoStack;
    IPlayer *mpFirstPlayer;
    IPlayer *mpSecondPlayer;
};

////////////////////////////////////////////////////////////////////////////////

DotTable::Private::Private(std::size_t width, std::size_t height, char firstOwner)
    : mWidth(width)
    , mHeight(height)
    , mFirstPlayer(firstOwner)
    , mpUndoStack(nullptr)
    , mpFirstPlayer(nullptr)
    , mpSecondPlayer(nullptr)
{
}

////////////////////////////////////////////////////////////////////////////////

DotTable::DotTable(std::size_t width, std::size_t height, char firstOwner)
    : m_p(new Private(width, height, firstOwner))
{
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
        m_p->mpFirstPlayer = player;
        break;
    case Cell::SECOND_OWNER:
        m_p->mpSecondPlayer = player;
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
        m_p->mpFirstPlayer = nullptr;
        break;
    case Cell::SECOND_OWNER:
        m_p->mpSecondPlayer = nullptr;
        break;
    default:
        assert(0);
    }
    
    player->unsetTable();
}

////////////////////////////////////////////////////////////////////////////////

std::size_t DotTable::width() const
{
    return m_p->mWidth;
}

////////////////////////////////////////////////////////////////////////////////

std::size_t DotTable::height() const
{
    return m_p->mHeight;
}

////////////////////////////////////////////////////////////////////////////////

void DotTable::setPlayingStrategy(std::unique_ptr<IPlayingStrategy> strategy)
{
    strategy.swap(m_p->mPlayingStategy);
}

////////////////////////////////////////////////////////////////////////////////

const Cell* DotTable::cell(std::size_t x, std::size_t y) const
{
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////

void DotTable::put(std::size_t x, std::size_t y)
{
}

////////////////////////////////////////////////////////////////////////////////

void DotTable::putFirstPlayer(std::size_t x, std::size_t y)
{
}

////////////////////////////////////////////////////////////////////////////////

void DotTable::putSecondPlayer(std::size_t x, std::size_t y)
{
}

////////////////////////////////////////////////////////////////////////////////

void DotTable::registerUndoManager(UndoStack *stack)
{
    m_p->mpUndoStack = stack;
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
