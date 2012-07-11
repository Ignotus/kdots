CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT (kdots)
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

FIND_PACKAGE (Qt4 REQUIRED)


SET (QT_USE_QTGUI TRUE)

INCLUDE (${QT_USE_FILE})


INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

SET (SRCS
  graphpoint.cpp
  graph.cpp
  dottable.cpp
  stepqueue.cpp
  tablewidget.cpp
  main.cpp
  newgamedialog.cpp
  mainwindow.cpp
  polygonfinder.cpp
  polygon.cpp
)

SET (HEADERS
  include/graphpoint.hpp
  include/graph.hpp
  include/dottable.hpp
  include/stepqueue.hpp
  include/tablewidget.hpp
  include/edgelist.hpp
  include/point.hpp
  include/newgamedialog.hpp
  include/mainwindow.hpp
  include/constants.hpp
  include/polygonfinder.hpp
  include/polygon.hpp
)

SET (FORMS
  ui/mainwindow.ui
  ui/newgamedialog.ui
)

QT4_WRAP_CPP (HEADERS_MOC ${HEADERS})
QT4_WRAP_UI (FORMS_HEADERS ${FORMS})

ADD_EXECUTABLE (kdots
  ${SRCS}
  ${HEADERS_MOC}
  ${FORMS_HEADERS}
)

TARGET_LINK_LIBRARIES (kdots
  ${QT_LIBRARIES}
)
