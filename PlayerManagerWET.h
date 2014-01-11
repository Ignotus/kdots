#pragma once
#include "IPlayerManager.h"

class PlayerMangerWET : public IPlayerManager
{
    int m_player;
public:
    inline PlayerMangerWET()
    {
        m_player = 0;
    }
    
    inline int nextPlayer(bool is_captured)
    {
        return (m_player = (m_player == 0 ? 1 : 0));
    }
    
    inline int currentPlayer() const
    {
        return m_player;
    }
};