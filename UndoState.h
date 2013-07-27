#pragma once
#include <stack>
#include <memory>

class UndoObject;
class UndoState
{
public:
    void addUndoObject(std::shared_ptr< UndoObject >& undoObject);
    
    void undo();
private:
    std::stack< std::shared_ptr< UndoObject > > mUndoObjects;
};