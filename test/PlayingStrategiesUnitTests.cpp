#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE DotTableUnitTests 
#include <boost/test/unit_test.hpp>
#include "DotTable.h"
#include "WithExtraMovePlayingStrategy.h"
#include "WithoutExtraMovePlayingStrategy.h"

const std::size_t WIDTH = 2;
const std::size_t HEIGHT = 3;

BOOST_AUTO_TEST_CASE(WithoutExtraMovePlayingStrategyTest)
{
    WithoutExtraMovePlayingStrategy s1;
    s1.init(Cell::FIRST_OWNER);
        
    BOOST_CHECK(s1.nextTurn(1) == Cell::SECOND_OWNER);
    BOOST_CHECK(s1.nextTurn(1) == Cell::FIRST_OWNER);
    BOOST_CHECK(s1.nextTurn(0) == Cell::SECOND_OWNER);
    BOOST_CHECK(s1.nextTurn(0) == Cell::FIRST_OWNER);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(WithExtraMovePlayingStrategyTest)
{
    WithExtraMovePlayingStrategy s2;
    s2.init(Cell::FIRST_OWNER);
    
    BOOST_CHECK(s2.nextTurn(1) == Cell::FIRST_OWNER);
    BOOST_CHECK(s2.nextTurn(1) == Cell::FIRST_OWNER);
    BOOST_CHECK(s2.nextTurn(0) == Cell::SECOND_OWNER);
    BOOST_CHECK(s2.nextTurn(0) == Cell::FIRST_OWNER);
}