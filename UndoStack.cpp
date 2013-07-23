#include "UndoStack.h"

UndoStack::UndoStack()
{
}

void UndoStack::addState(std::shared_ptr< UndoState >& state)
{
    mUndoStates.push(state);
}
    
bool UndoStack::undo()
{
    return false;
}