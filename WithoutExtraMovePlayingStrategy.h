#pragma once

#include "IPlayingStrategy.h"
#include "WithExtraMovePlayingStrategy.h"

class WithoutExtraMovePlayingStrategy : public IPlayingStrategy
{
    WithExtraMovePlayingStrategy mPlayingStrategy;
public:
    virtual char nextPlayer(bool isCaptured) const;
    virtual void init(char firstPlayer);
    virtual char nextTurn(bool isCaptured);
};