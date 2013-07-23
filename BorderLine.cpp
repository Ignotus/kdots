#include "BorderLine.h"

BorderLine::BorderLine()
    : mDx(0)
    , mDy(0)
    , mIsLeftCaptured(false)
    , mIsRightCaptured(false)
{
}

BorderLine::BorderLine(signed char dx, signed char dy)
    : mDx(dx)
    , mDy(dy)
    , mIsLeftCaptured(false)
    , mIsRightCaptured(false)
{
#ifdef DEBUG
    assert(mDx >= -1 && mDx <= 1 && mDy >= -1 && mDy <= 1);
#endif
}

bool BorderLine::isLeftSideCaptured() const
{
    return mIsLeftCaptured;
}

bool BorderLine::isRightSideCaptured() const
{
    return mIsRightCaptured;
}

void BorderLine::setLeftCaptured()
{
    mIsLeftCaptured = true;
}

void BorderLine::setRightCaptured()
{
    mIsRightCaptured = true;
}

signed char BorderLine::dx() const
{
    return mDx;
}

signed char BorderLine::dy() const
{
    return mDy;
}

bool BorderLine::operator==(const BorderLine& other) const
{
    return mDx == other.mDx && mDy == other.mDy;
}

bool BorderLine::operator!=(const BorderLine& other) const
{
    return !operator==(other);
}