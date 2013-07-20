#include "Cell.h"

Cell::Cell()
    : mOwner(NO_OWNER)
{

}

char Cell::owner() const
{
    return mOwner;
}