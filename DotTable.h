#pragma once
#include <memory>
#include <vector>
#include "Cell.h"

class IPlayingStrategy;

class DotTable
{
    const std::size_t mWidth;
    const std::size_t mHeight;
    const char mFirstPlayer;
    std::vector< std::vector< Cell > > mData;
    std::unique_ptr<IPlayingStrategy> mPlayingStategy;
public:
    DotTable(std::size_t width, std::size_t height, char firstPlayer = Cell::FIRST_OWNER);
    ~DotTable();
    
    std::size_t width() const;
    std::size_t height() const;
    
    void setPlayingStrategy(std::unique_ptr<IPlayingStrategy> strategy);
    
    const Cell* cell(std::size_t x, std::size_t y) const;
    
    void put(std::size_t x, std::size_t y);
    void putFirstPlayer(std::size_t x, std::size_t y);
    void putSecondPlayer(std::size_t x, std::size_t y);
    
    bool operator==(const DotTable& other) const;
};

std::ostream& operator<<(std::ostream& out, const DotTable& table);
std::istream& operator>>(std::istream& in, DotTable& table);