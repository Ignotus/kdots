CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT (kdots)

FIND_PACKAGE (Qt4 REQUIRED)
#FIND_PACKAGE (KDE4)

SET (QT_USE_QTGUI TRUE)
SET (QT_USE_QTCORE TRUE)

INCLUDE (${QT_USE_FILE})

#IF (KDE4_FOUND)
#	ADD_DEFINITIONS (-DKDE4_FOUND)
#	include (KDE4Defaults)
#	INCLUDE_DIRECTORIES (${KDE4_INCLUDES})
#ENDIF (KDE4_FOUND)

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

SET (PLUGIN_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/plugins)
SET (KDOTS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

SET (INSTALL_PREFIX "/usr/local")

IF (UNIX)
	SET (LIBDIR ${INSTALL_PREFIX}/lib${LIB_SUFFIX})
	SET (PLUGINS_DIR ${LIBDIR}/kdots/plugins)
	SET (BINDIR bin)
	ADD_DEFINITIONS (-DPLUGINS_DIR="${PLUGINS_DIR}")
ENDIF (UNIX)

SET (SRCS
  graphpoint.cpp
  graph.cpp
  dottable.cpp
  stepqueue.cpp
  tablewidget.cpp
  main.cpp
  newgamewidget.cpp
  newgamedialog.cpp
  mainwindow.cpp
  polygonfinder.cpp
  polygon.cpp
  plugincontainer.cpp
  pluginmanagerwidget.cpp
  gameconfig.cpp
  point.cpp
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
  include/newgamewidget.hpp
  include/mainwindow.hpp
  include/constants.hpp
  include/polygonfinder.hpp
  include/polygon.hpp
  include/plugincontainer.hpp
  include/pluginmanagerwidget.hpp
  include/gameconfig.hpp
)

SET (INTERFACE_HEAD
  interface/iplugin.hpp
  interface/irival.hpp
  interface/iconfigurationwidget.hpp
)

SET (FORMS
  ui/mainwindow.ui
  ui/newgamewidget.ui
  ui/newgamedialog.ui
  ui/pluginmanagerwidget.ui
)

QT4_WRAP_CPP (HEADERS_MOC ${HEADERS})
QT4_WRAP_CPP (INTERFACE_MOC ${INTERFACE_HEAD})
QT4_WRAP_UI (FORMS_HEADERS ${FORMS})

ADD_EXECUTABLE (kdots
  ${SRCS}
  ${HEADERS_MOC}
  ${INTERFACE_MOC}
  ${FORMS_HEADERS}
)

#IF (KDE4_FOUND)
#	TARGET_LINK_LIBRARIES (kdots
#	  ${KDE4_KDEUI_LIBS}
#	  ${KDE4_KPARTS_LIBS}
#	)
#ELSE (KDE4_FOUND)
	TARGET_LINK_LIBRARIES (kdots
	  ${QT_LIBRARIES}
	)
#ENDIF (KDE4_FOUND)
	
	
IF (UNIX)
	INSTALL (TARGETS kdots DESTINATION ${BINDIR})
ENDIF (UNIX)

# uninstall target
CONFIGURE_FILE (
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET (uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

ADD_SUBDIRECTORY (plugins)