#pragma once
#include <stack>
#include <memory>
#include "UndoState.h"

class UndoStack
{
    std::stack< std::shared_ptr<UndoState> > mUndoStates;
public:
    UndoStack();
    
    void addState(std::shared_ptr< UndoState >& state);
    
    bool undo();
};