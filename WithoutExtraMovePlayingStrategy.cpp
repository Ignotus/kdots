#include "WithoutExtraMovePlayingStrategy.h"

char WithoutExtraMovePlayingStrategy::nextPlayer(bool isCaptured) const
{
    return mPlayingStrategy.nextPlayer(0);
}

////////////////////////////////////////////////////////////////////////////////

void WithoutExtraMovePlayingStrategy::init(char firstPlayer)
{
    mPlayingStrategy.init(firstPlayer);
}

////////////////////////////////////////////////////////////////////////////////

char WithoutExtraMovePlayingStrategy::nextTurn(bool isCaptured)
{
    return mPlayingStrategy.nextTurn(0);
}
