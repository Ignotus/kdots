#pragma once

class IPlayerManager
{
public:
    virtual ~IPlayerManager() {}
    
    virtual int nextPlayer(bool is_captured) = 0;
    
    virtual int currentPlayer() const = 0;
};