#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE DotTableUnitTests 
#include <boost/test/unit_test.hpp>
#include "DotTable.h"
#include "WithExtraMovePlayingStrategy.h"
#include "WithoutExtraMovePlayingStrategy.h"

const std::size_t WIDTH = 3;
const std::size_t HEIGHT = 3;

BOOST_AUTO_TEST_CASE(InitializationTest)
{
    DotTable table(WIDTH, HEIGHT);
    BOOST_CHECK_EQUAL(table.height(), WIDTH);
    BOOST_CHECK_EQUAL(table.width(), HEIGHT);
    
    BOOST_CHECK(table.cell(WIDTH + 2, HEIGHT + 2) == nullptr);
    
    for (std::size_t i = 0; i < WIDTH; ++i)
    {
        for (std::size_t j = 0; j < HEIGHT; ++j)
        {
            const Cell *pCell = table.cell(i, j);
            BOOST_REQUIRE(pCell != nullptr);
            
            BOOST_CHECK(pCell->owner() == Cell::NO_OWNER);
        }
    }
}

void checkStrategy(IPlayingStrategy *strategy)
{
    DotTable table(WIDTH, HEIGHT, Cell::FIRST_OWNER);
    
    table.setPlayingStrategy(std::unique_ptr<IPlayingStrategy>(strategy));
    table.put(0, 0);
    
    BOOST_CHECK(table.cell(0, 0)->owner() == Cell::FIRST_OWNER);
    table.put(0, 0);
    BOOST_CHECK(table.cell(0, 0)->owner() == Cell::FIRST_OWNER);
    table.put(1, 0);
    BOOST_CHECK(table.cell(1, 0)->owner() == Cell::SECOND_OWNER);
    table.put(0, 1);
    BOOST_CHECK(table.cell(0, 1)->owner() == Cell::FIRST_OWNER);
    
    table.putFirstPlayer(1, 1);
    BOOST_CHECK(table.cell(1, 1)->owner() == Cell::NO_OWNER);
    
    table.putSecondPlayer(1, 1);
    BOOST_CHECK(table.cell(1, 1)->owner() == Cell::SECOND_OWNER);
    
    table.putSecondPlayer(2, 2);
    BOOST_CHECK(table.cell(2, 2)->owner() == Cell::NO_OWNER);
    
    table.putFirstPlayer(2, 2);
    BOOST_CHECK(table.cell(2, 2)->owner() == Cell::FIRST_OWNER);
}

BOOST_AUTO_TEST_CASE(PlayingStrategiesTest)
{
    checkStrategy(new WithoutExtraMovePlayingStrategy);
    checkStrategy(new WithExtraMovePlayingStrategy);
}