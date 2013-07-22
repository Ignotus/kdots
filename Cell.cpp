#include "Cell.h"

Cell::Cell()
    : mOwner(NO_OWNER)
{

}

char Cell::owner() const
{
    return mOwner;
}

void Cell::setOwner(Owner owner)
{
    mOwner = owner;
}

void Cell::addBorder(const BorderLine& border)
{
}

void Cell::removerBorder(const BorderLine& border)
{
}

bool Cell::hasBorder(const BorderLine& border) const
{
    return mBorders.count(border) != 0;
}

std::unordered_set<BorderLine>::const_iterator Cell::beginBorders() const
{
    return mBorders.begin();
}

std::unordered_set<BorderLine>::const_iterator Cell::endBorders() const
{
    return mBorders.end();
}