#pragma once
#include <functional>

class BorderLine
{
    const signed char mDx;
    const signed char mDy;
    bool mIsLeftCaptured;
    bool mIsRightCaptured;
public:
    BorderLine();
    BorderLine(signed char dx, signed char dy);
    
    bool isLeftSideCaptured() const;
    bool isRightSideCaptured() const;
    
    void setLeftCaptured();
    void setRightCaptured();
    
    signed char dx() const;
    signed char dy() const;
    
    bool operator==(const BorderLine& other) const;
    bool operator!=(const BorderLine& other) const;
};

namespace std
{
    template<>
    class hash<BorderLine>
    {
    public:
        std::size_t operator()(const BorderLine& s) const 
        {
            const std::size_t h1 = std::hash<signed char>()(s.dx());
            const std::size_t h2 = std::hash<signed char>()(s.dy());
            return h1 ^ (h2 << 1);
        }
    };
}