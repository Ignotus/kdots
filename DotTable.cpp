#include <cassert>
#include "DotTable.h"
#include "IPlayingStrategy.h"
#include "IPlayer.h"

DotTable::DotTable(std::size_t width, std::size_t height, char firstOwner)
    : mWidth(width)
    , mHeight(height)
    , mFirstPlayer(firstOwner)
    , mpUndoStack(nullptr)
    , mpFirstPlayer(nullptr)
    , mpSecondPlayer(nullptr)
{
}

DotTable::~DotTable()
{
}

void DotTable::registerPlayer(IPlayer *player)
{
    switch (player->player())
    {
    case Cell::FIRST_OWNER:
        mpFirstPlayer = player;
        break;
    case Cell::SECOND_OWNER:
        mpSecondPlayer = player;
        break;
    default:
        assert(0);
    }
}

void DotTable::unregisterPlayer(IPlayer *player)
{
    switch (player->player())
    {
    case Cell::FIRST_OWNER:
        mpFirstPlayer = nullptr;
        break;
    case Cell::SECOND_OWNER:
        mpSecondPlayer = nullptr;
        break;
    default:
        assert(0);
    }
}

std::size_t DotTable::width() const
{
    return mWidth;
}

std::size_t DotTable::height() const
{
    return mHeight;
}

void DotTable::setPlayingStrategy(std::unique_ptr<IPlayingStrategy> strategy)
{
    strategy.swap(mPlayingStategy);
}

const Cell* DotTable::cell(std::size_t x, std::size_t y) const
{
    return nullptr;
}

void DotTable::put(std::size_t x, std::size_t y)
{
}

void DotTable::putFirstPlayer(std::size_t x, std::size_t y)
{
}

void DotTable::putSecondPlayer(std::size_t x, std::size_t y)
{
}

void DotTable::registerUndoManager(UndoStack *stack)
{
    mpUndoStack = stack;
}

bool DotTable::operator==(const DotTable& other) const
{
    return false;
}

bool DotTable::operator!=(const DotTable& other) const
{
    return true;
}

std::ostream& operator<<(std::ostream& out, const DotTable& table)
{
    return out;
}

std::istream& operator>>(std::istream& in, DotTable& table)
{
    return in;
}
