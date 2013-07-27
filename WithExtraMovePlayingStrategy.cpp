#include <cassert>
#include "WithExtraMovePlayingStrategy.h"
#include "Cell.h"

WithExtraMovePlayingStrategy::WithExtraMovePlayingStrategy()
    : mCurrentPlayer(Cell::NO_OWNER)
{
}

////////////////////////////////////////////////////////////////////////////////

namespace
{
    char otherPlayer(char currentPlayer)
    {
        switch (currentPlayer)
        {
        case Cell::FIRST_OWNER:
            return Cell::SECOND_OWNER;
        case Cell::SECOND_OWNER:
            return Cell::FIRST_OWNER;
        default:
            assert(0);
        }
        
        return Cell::NO_OWNER;
    }
}

////////////////////////////////////////////////////////////////////////////////

char WithExtraMovePlayingStrategy::nextPlayer(bool isCaptured) const
{
    return isCaptured ? mCurrentPlayer : otherPlayer(mCurrentPlayer);
}

////////////////////////////////////////////////////////////////////////////////

void WithExtraMovePlayingStrategy::init(char firstPlayer)
{
    mCurrentPlayer = firstPlayer;
}

////////////////////////////////////////////////////////////////////////////////

char WithExtraMovePlayingStrategy::nextTurn(bool isCaptured)
{
    mCurrentPlayer = nextPlayer(isCaptured);
    return mCurrentPlayer;
}

////////////////////////////////////////////////////////////////////////////////

char WithExtraMovePlayingStrategy::currentPlayer() const
{
    return mCurrentPlayer;
}

