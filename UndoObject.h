#pragma once

class UndoObject
{
public:
    virtual ~UndoObject();
    
    virtual void undo() = 0;
};