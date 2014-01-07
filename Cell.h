#pragma once

class Cell
{
public:
    inline Cell()
        : m_owner(-1)
        , m_captured(false)
    {
    }

    inline void capture()
    {
        m_captured = true;
    }
    
    inline bool isCaptured() const
    {
        return m_captured;
    }

public:
    int m_owner;

private:
    bool m_captured;
};