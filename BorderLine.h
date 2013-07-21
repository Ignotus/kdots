#pragma once

class BorderLine
{
    const signed char mDx;
    const signed char mDy;
public:
    BorderLine();
    BorderLine(signed char dx, signed char dy);
    
    bool isLeftSideCaptured() const;
    bool isRightSideCaptured() const;
    
};