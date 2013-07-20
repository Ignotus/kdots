#include "DotTable.h"
#include "IPlayingStrategy.h"

DotTable::DotTable(std::size_t width, std::size_t height, char firstOwner)
    : mWidth(width)
    , mHeight(height)
    , mFirstPlayer(firstOwner)
{
}

DotTable::~DotTable()
{
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

bool operator==(const DotTable& other) const
{
    return false;
}

std::ostream& operator<<(std::ostream& out, const DotTable& table)
{
    return out;
}

std::istream& operator>>(std::istream& in, DotTable& table)
{
    return in;
}