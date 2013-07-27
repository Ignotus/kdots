#pragma once
#include <stack>
#include <memory>

class UndoState;
class UndoStack
{
public:
    UndoStack();
    
    void addState(std::shared_ptr< UndoState >& state);
    
    bool undo();
    
private:
    std::stack< std::shared_ptr<UndoState> > mUndoStates;
};