#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE BorderLineUnitTests
#include <boost/test/unit_test.hpp>
#include "BorderLine.h"

BOOST_AUTO_TEST_CASE(BorderLineTest)
{
    BorderLine border(1, 0);
    
    BOOST_CHECK(border.dx() == 1);
    BOOST_CHECK(border.dy() == 0);
    
    BOOST_CHECK(border.isLeftSideCaptured() == false);
    BOOST_CHECK(border.isRightSideCaptured() == false);
    
    border.setLeftCaptured();
    BOOST_CHECK(border.isLeftSideCaptured() == true);

    border.setRightCaptured();
    BOOST_CHECK(border.isRightSideCaptured() == true);
    
    BorderLine border1(0, 1);
    
    BOOST_CHECK(border1 != border);
    BOOST_CHECK(border1 == border1);
}