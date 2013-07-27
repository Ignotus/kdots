#pragma once
#include <memory>
#include <vector>
#include "Cell.h"

class UndoStack;
class IPlayingStrategy;
class IPlayer;

class DotTable
{
private:
    DotTable(const DotTable&);
    const DotTable& operator=(const DotTable&);
    
public:
    DotTable(std::size_t width, std::size_t height, char firstPlayer = Cell::FIRST_OWNER);
    ~DotTable();
    
    std::size_t width() const;
    std::size_t height() const;
    
    void registerPlayer(IPlayer *player);
    void unregisterPlayer(IPlayer *player);
    
    void setPlayingStrategy(std::unique_ptr< IPlayingStrategy > strategy);
    
    const Cell* cell(std::size_t x, std::size_t y) const;
    
    
    bool putFirstPlayer(std::size_t x, std::size_t y);
    bool putSecondPlayer(std::size_t x, std::size_t y);
    
    bool operator==(const DotTable& other) const;
    bool operator!=(const DotTable& other) const;
    
    void registerUndoManager(UndoStack *stack);

private:
    class Private;
    
    std::unique_ptr< Private > m_p;
};

std::ostream& operator<<(std::ostream& out, const DotTable& table);
std::istream& operator>>(std::istream& in, DotTable& table);