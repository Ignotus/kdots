cmake_minimum_required(VERSION 2.6)
project(kdots)

add_definitions(-std=c++0x -Wall -fexceptions -fopenmp)

find_package(Qt4 REQUIRED)
find_package(KDE4)


if(KDE4_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

set(QT_USE_QTGUI TRUE)
set(QT_USE_QTCORE TRUE)
set(QT_USE_NETWORK TRUE)

include_directories(
    ${QT_INCLUDES}
)

set(SRCS
    Board.cpp
)

set(HDRS
    Board.h
    Cell.h
)

add_library(kdotslib ${SRCS} ${HDRS})

add_executable(kdots  main.cpp)

target_link_libraries(kdots kdotslib)
