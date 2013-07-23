#pragma once

#include "IPlayingStrategy.h"

class WithExtraMovePlayingStrategy : public IPlayingStrategy
{
    char mCurrentPlayer;
public:
    WithExtraMovePlayingStrategy();
    virtual char nextPlayer(bool isCaptured) const;
    virtual void init(char firstPlayer);
    virtual char nextTurn(bool isCaptured);
};