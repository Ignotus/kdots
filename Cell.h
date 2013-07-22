#pragma once
#include <unordered_set>
#include "BorderLine.h"

class Cell
{
    char mOwner;
    std::unordered_set<BorderLine> mBorders;
public:
    enum Owner
    {
        NO_OWNER = 0,
        FIRST_OWNER,
        SECOND_OWNER
    };
    
    Cell();
    
    char owner() const;
    void setOwner(Owner owner);
    
    void addBorder(const BorderLine& border);
    void removerBorder(const BorderLine& border);
    
    bool hasBorder(const BorderLine& border) const;
    
    std::unordered_set<BorderLine>::const_iterator beginBorders() const;
    std::unordered_set<BorderLine>::const_iterator endBorders() const;
};