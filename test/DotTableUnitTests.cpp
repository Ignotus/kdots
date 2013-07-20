#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE DotTableUnitTests 
#include <boost/test/unit_test.hpp>
#include "DotTable.h"

BOOST_AUTO_TEST_CASE(InitializationTest)
{
    const std::size_t width = 2;
    const std::size_t height = 3;
    DotTable table(width, height);
    BOOST_CHECK_EQUAL(table.height(), width);
    BOOST_CHECK_EQUAL(table.width(), height);
    
    BOOST_CHECK(table.cell(width + 2, height + 2) == nullptr);
    
    for (std::size_t i = 0; i < width; ++i)
    {
        for (std::size_t j = 0; j < height; ++j)
        {
            const Cell *pCell = table.cell(i, j);
            BOOST_REQUIRE(pCell != nullptr);
            
            BOOST_CHECK(pCell->owner() == Cell::NO_OWNER);
        }
    }
}