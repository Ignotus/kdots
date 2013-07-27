#pragma once

class Cell;

class IPlayingStrategy
{
public:
    virtual ~IPlayingStrategy() {}
    
    virtual void init(char firstPlayer) = 0;
    virtual char nextPlayer(bool isCaptured) const = 0;
    virtual char nextTurn(bool isCaptured) = 0;
    virtual char currentPlayer() const = 0;
};