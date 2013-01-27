cmake_minimum_required(VERSION 2.6)
project(kdots)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

add_definitions(-std=c++0x -Wall)

find_package(Qt4 REQUIRED)
find_package(KDE4 REQUIRED)

#find_package(LibKDEGames)

#if(KDEGAMES_FOUND)
#  add_definitions(-DOLD_LIBKDEGAMES)
#else(KDEGAMES_FOUND)
#  find_package(KDEGames REQUIRED)
#  message(STATUS "Found KDEGames")
#  add_definitions(-DNEW_LIBKDEGAMES)
#endif(KDEGAMES_FOUND)

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
  mainwindow.cpp
  main.cpp
  matrix.cpp
  tablemodel.cpp
  tableview.cpp
  pdata.cpp
  ownerdetector.cpp
  configuration.cpp
  polygonfinder.cpp
  polygondfsfinder.cpp
  pointcounter.cpp
)

set(HEADERS
  mainwindow.h
  matrix.h
  tablemodel.h
  tableview.h
  pdata.h
  ownerdetector.h
  configuration.h
  polygonfinder.h
  polygondfsfinder.h
  pointcounter.h
)

set(FORMS
  ui/mainwindow.ui
)

kde4_add_ui_files(SRCS ${FORMS})

kde4_add_executable(kdots ${SRCS})

target_link_libraries(kdots ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS} ${KDEGAMES_LIBRARY})
