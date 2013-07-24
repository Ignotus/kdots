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
    unregisterTable();
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

void IPlayer::registerTable(DotTable *table)
{
    mpTable = table;
    mpTable->registerPlayer(this);
}

void IPlayer::unregisterTable()
{
    if (mpTable != nullptr)
    {
        mpTable->unregisterPlayer(this);
        mpTable = nullptr;
    }
}