#include "UndoState.h"

void UndoState::addUndoObject(std::shared_ptr< UndoObject >& undoObject)
{
    mUndoObjects.push(undoObject);
}

void UndoState::undo()
{
    while (!mUndoObjects.empty())
    {
        std::shared_ptr< UndoObject >& topObject = mUndoObjects.top();
        topObject->undo();
        mUndoObjects.pop();
    }
}