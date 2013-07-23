#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE CellUnitTests
#include <boost/test/unit_test.hpp>
#include "Cell.h"

BOOST_AUTO_TEST_CASE(CellTest)
{
    Cell cell;
    
    BOOST_CHECK(cell.owner() == Cell::NO_OWNER);
    
    cell.setOwner(Cell::FIRST_OWNER);
    
    BOOST_CHECK(cell.owner() == Cell::FIRST_OWNER);
    
    BOOST_CHECK(cell.beginBorders() == cell.endBorders());
    
    cell.addBorder(BorderLine(1, 0));
    
    BOOST_CHECK(cell.hasBorder(BorderLine(1, 0)) == true);
    BOOST_CHECK(cell.hasBorder(BorderLine(0, 1)) == false);
    
    cell.removerBorder(BorderLine(1, 0));
    BOOST_CHECK(cell.hasBorder(BorderLine(1, 0)) == false);
    
    
    cell.addBorder(BorderLine(1, 0));
    cell.addBorder(BorderLine(1, 1));
    
    int count = 0;
    for (auto itr = cell.beginBorders(), end = cell.endBorders(); itr != end; ++itr)
    {
        ++count;
        BOOST_CHECK(itr->dx() == 1);
    }
    
    BOOST_CHECK(count == 2);
}