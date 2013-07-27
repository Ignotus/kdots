#pragma once
#include <functional>

class BorderLine
{
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
    
private:
    const signed char mDx;
    const signed char mDy;
    bool mIsLeftCaptured;
    bool mIsRightCaptured;
};

namespace std
{
    template<>
    class hash<BorderLine>
    {
    public:
        inline std::size_t operator()(const BorderLine& s) const
        {
            const std::size_t h1 = std::hash<signed char>()(s.dx());
            const std::size_t h2 = std::hash<signed char>()(s.dy());
            return h1 ^ (h2 << 1);
        }
    };
}