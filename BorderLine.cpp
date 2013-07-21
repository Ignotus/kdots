#include "BorderLine.h"

BorderLine::BorderLine()
    : mDx(0)
    , mDy(0)
{
}

BorderLine::BorderLine(signed char dx, signed char dy)
    : mDx(dx)
    , mDy(dy)
{
}

bool BorderLine::isLeftSideCaptured() const
{
    return false;
}

bool BorderLine::isRightSideCaptured() const
{
    return false;
}