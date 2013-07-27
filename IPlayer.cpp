#include <cassert>
#include "IPlayer.h"
#include "DotTable.h"

IPlayer::IPlayer(char player)
    : mpTable(nullptr)
    , mPlayer(player)
{
}

IPlayer::~IPlayer()
{
    unsetTable();
}

void IPlayer::put(std::size_t x, std::size_t y)
{
    if (mpTable == nullptr)
    {
        switch (mPlayer)
        {
        case Cell::FIRST_OWNER:
            mpTable->putFirstPlayer(x, y);
            break;
        case Cell::SECOND_OWNER:
            mpTable->putSecondPlayer(x, y);
            break;
        default:
            assert(0);
        }
            
    }
}

char IPlayer::player() const
{
    return mPlayer;
}

const DotTable* IPlayer::dotTable() const
{
    return mpTable;
}

void IPlayer::setTable(DotTable *table)
{
    mpTable = table;
}

void IPlayer::unsetTable()
{
    mpTable = nullptr;
}