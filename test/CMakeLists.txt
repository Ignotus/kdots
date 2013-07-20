find_package(Boost COMPONENTS unit_test_framework REQUIRED)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../
)

file(GLOB TEST_SRCS "*.cpp")

foreach(FILE ${TEST_SRCS})
    string(REGEX MATCH ".*/([^/.]+)[.]cpp" FILE ${FILE})
    set(TEST_NAME ${CMAKE_MATCH_1})
    message(STATUS "Adding a unit test: ${TEST_NAME}")
    add_executable(${TEST_NAME} ${FILE})
    add_test(${TEST_NAME} ${TEST_NAME})
    target_link_libraries(${TEST_NAME} ${Boost_LIBRARIES} kdotslib)
endforeach()