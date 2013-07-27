#include <cassert>
#include "WithExtraMovePlayingStrategy.h"
#include "Cell.h"

WithExtraMovePlayingStrategy::WithExtraMovePlayingStrategy()
    : mCurrentPlayer(Cell::NO_OWNER)
{
}

////////////////////////////////////////////////////////////////////////////////

char WithExtraMovePlayingStrategy::nextPlayer(bool isCaptured) const
{
    assert(mCurrentPlayer != Cell::NO_OWNER);
    return Cell::NO_OWNER;
}

////////////////////////////////////////////////////////////////////////////////

void WithExtraMovePlayingStrategy::init(char firstPlayer)
{

}

////////////////////////////////////////////////////////////////////////////////

char WithExtraMovePlayingStrategy::nextTurn(bool isCaptured)
{
    mCurrentPlayer = nextPlayer(isCaptured);
    return mCurrentPlayer;
}

