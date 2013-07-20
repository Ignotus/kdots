#include "DotTable.h"

DotTable::DotTable(std::size_t width, std::size_t height)
    : mWidth(width)
    , mHeight(height)
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

const Cell* DotTable::cell(std::size_t x, std::size_t y) const
{
    return nullptr;
}

void DotTable::put(std::size_t x, std::size_t y)
{
}