cmake_minimum_required(VERSION 2.6)
project(kdots)

add_definitions(-std=c++0x -Wall -fexceptions)

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
    BorderLine.cpp
    WithExtraMovePlayingStrategy.cpp
    WithoutExtraMovePlayingStrategy.cpp
    Cell.cpp
    DotTable.cpp
)

set(HDRS 
    BorderLine.h
    WithExtraMovePlayingStrategy.h
    WithoutExtraMovePlayingStrategy.h
    Cell.h
    DotTable.h
    IPlayingStrategy.h
    
)

add_library(kdotslib ${SRCS} ${HDRS})

add_executable(kdots  main.cpp)

target_link_libraries(kdots kdotslib)


