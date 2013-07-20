#pragma once
#include <vector>
#include "Cell.h"

class DotTable
{
    std::size_t mWidth;
    std::size_t mHeight;
    std::vector< std::vector< Cell > > mData;
public:
    DotTable(std::size_t width, std::size_t height);
    ~DotTable();
    
    std::size_t width() const;
    std::size_t height() const;
    
    const Cell* cell(std::size_t x, std::size_t y) const;
    
    void put(std::size_t x, std::size_t y);
};