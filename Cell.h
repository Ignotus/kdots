#pragma once

class Cell
{
    char mOwner;
public:
    enum Owner
    {
        NO_OWNER = 0,
        FIRST_OWNER,
        SECOND_OWNER
    };
    
    Cell();
    
    char owner() const;
};