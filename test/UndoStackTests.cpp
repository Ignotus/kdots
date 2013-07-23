#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE UndoStackTests
#include <boost/test/unit_test.hpp>
#include "DotTable.h"
#include "UndoStack.h"

BOOST_AUTO_TEST_CASE(UndoStackTest)
{
    UndoStack stack;
    DotTable table(5, 5);
    table.registerUndoManager(&stack);

    const int points [4][2] = {{0, 1},
                               {1, 0},
                               {2, 1},
                               {1, 2}};
    for (int i = 0; i < 4; ++i)
    {
        table.put(points[i][0], points[i][1]);
    }
    
    for (int i = 0; i < 4; ++i)
    {
        const Cell *ccell = table.cell(points[i][0], points[i][1]);
        BOOST_CHECK(ccell->beginBorders() != ccell->endBorders());
    }
    
    BOOST_CHECK(stack.undo() == true);
    BOOST_CHECK(table.cell(points[0][0], points[0][1])->owner() == Cell::NO_OWNER);
    for (int i = 0; i < 4; ++i)
    {
        const Cell *ccell = table.cell(points[i][0], points[i][1]);
        BOOST_CHECK(ccell->beginBorders() == ccell->endBorders());
    }
    
    for (int i = 1; i < 4; ++i)
    {
        BOOST_CHECK(stack.undo() == true);
        BOOST_CHECK(table.cell(points[i][0], points[i][1])->owner() == Cell::NO_OWNER);
    }
    
    BOOST_CHECK(stack.undo() == false);
}