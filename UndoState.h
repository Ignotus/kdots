#pragma once
#include <stack>
#include <memory>
#include "UndoObject.h"

class UndoState
{
    std::stack< std::shared_ptr< UndoObject > > mUndoObjects;
public:
    void addUndoObject(std::shared_ptr< UndoObject >& undoObject);
    
    void undo();
};