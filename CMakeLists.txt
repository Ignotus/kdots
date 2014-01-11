cmake_minimum_required(VERSION 2.6)
project(kdots)

add_definitions(-std=c++0x -Wall -fexceptions)

find_package(Qt4 REQUIRED)

set(QT_USE_QTGUI TRUE)
set(QT_USE_QTCORE TRUE)
set(QT_USE_NETWORK TRUE)

include(${QT_USE_FILE})

include_directories(
    ${QT_INCLUDES}
)

set(SRCS
    BorderFinder.cpp
    Board.cpp
    BoardView.cpp
    MainWindow.cpp
)

set(HDRS
    BorderFinder.h
    Board.h
    BoardView.h
    Cell.h
    MainWindow.h
    ScoreCounter.h
)

QT4_WRAP_CPP(HEADERS_MOC ${HDRS})

add_executable(kdots ${SRCS} ${HDRS} ${HEADERS_MOC} main.cpp)

target_link_libraries(
    kdots
    ${QT_LIBRARIES}
)
