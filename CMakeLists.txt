cmake_minimum_required(VERSION 2.6)
project(kdots)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall")

find_package(Qt4 REQUIRED)
find_package(KDE4 REQUIRED)

find_package(LibKDEGames)

if(KDEGAMES_FOUND)
  add_definitions(-DOLD_LIBKDEGAMES)
else(KDEGAMES_FOUND)
  find_package(KDEGames REQUIRED)
  message(STATUS "Found KDEGames")
  add_definitions(-DNEW_LIBKDEGAMES)
endif(KDEGAMES_FOUND)

include(KDE4Defaults)

set(QT_USE_QTGUI TRUE)
set(QT_USE_QTCORE TRUE)

include(${QT_USE_FILE})

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${KDE4_INCLUDES}
  ${KDEGAMES_INCLUDE_DIRS}
  ${QT_INCLUDES}
)

set(SRCS
  main.cpp
  matrix.cpp
  tablemodel.cpp
  tableview.cpp
  pdata.cpp
  ownerdetector.cpp
  configuration.cpp
)

set(HEADERS
  matrix.h
  tablemodel.h
  tableview.h
  pdata.h
  ownerdetector.h
  configuration.h
)

kde4_add_executable(kdots ${SRCS})

target_link_libraries(kdots ${QT_LIBRARIES})
