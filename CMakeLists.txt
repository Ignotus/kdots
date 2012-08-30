CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (kdots)

SET (CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH}
	${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

FIND_PACKAGE (Qt4 REQUIRED)
FIND_PACKAGE (KDE4 REQUIRED)

FIND_PACKAGE (LibKDEGames)

IF (KDEGAMES_FOUND)
	ADD_DEFINITIONS(-DOLD_LIBKDEGAMES)
ELSE (KDEGAMES_FOUND)
	FIND_PACKAGE (KDEGames REQUIRED)
	MESSAGE(STATUS "Found KDEGames")
	ADD_DEFINITIONS (-DNEW_LIBKDEGAMES)
ENDIF (KDEGAMES_FOUND)

INCLUDE (KDE4Defaults)

OPTION (KDE4_BUILD_TESTS "Enable test units" OFF)

IF (KDE4_BUILD_TESTS)
	ENABLE_TESTING ()
	SET (QT_USE_QTTEST TRUE)
ENDIF (KDE4_BUILD_TESTS)

SET (QT_USE_QTGUI TRUE)
SET (QT_USE_QTCORE TRUE)

INCLUDE (${QT_USE_FILE})

MESSAGE( STATUS "${KDEGAMES_INCLUDE_DIRS}")

INCLUDE_DIRECTORIES (
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${KDE4_INCLUDES}
	${KDEGAMES_INCLUDE_DIRS}
	${QT_INCLUDES}
)

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -fexceptions")

SET (PLUGIN_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/plugins)
SET (KDOTS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

SET (INSTALL_PREFIX "/usr/local")

IF (UNIX)
	SET (PLUGINS_DIR ${DATA_INSTALL_DIR}/kdots/plugins)
ENDIF (UNIX)

SET (SRCS
	brushcombo.cpp
	graph.cpp
	dottable.cpp
	stepqueue.cpp
	tablewidget.cpp
	newgamewidget.cpp
	newgamedialog.cpp
	mainwindow.cpp
	polygonfinder.cpp
	plugincontainer.cpp
	pluginmanagerwidget.cpp
	gameconfig.cpp
	point.cpp
	brushcombodelegate.cpp
	kdots.kcfgc
)

SET (HEADERS
	brushcombo.hpp
	graphpoint.hpp
	graph.hpp
	dottable.hpp
	stepqueue.hpp
	tablewidget.hpp
	edgelist.hpp
	point.hpp
	newgamedialog.hpp
	newgamewidget.hpp
	mainwindow.hpp
	constants.hpp
	polygonfinder.hpp
	polygon.hpp
	plugincontainer.hpp
	pluginmanagerwidget.hpp
	gameconfig.hpp
	brushcombodelegate.hpp
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
	ui/boardconfigwidget.ui
)

KDE4_ADD_UI_FILES (SRCS ${FORMS})
KDE4_ADD_KCFG_FILES (SRCS kdots.kcfgc)
INSTALL (FILES kdots.kcfg DESTINATION ${KCFG_INSTALL_DIR})

CONFIGURE_FILE (config.hpp.in config.hpp)

SET (KDOTS_REQUIRED_LIBS
	${KDE4_KDECORE_LIBS}
	${KDE4_KDEUI_LIBS}
	${KDEGAMES_LIBRARY}
	${QT_LIBRARIES}
)

KDE4_ADD_LIBRARY (kdotslib SHARED
	${SRCS}
)

KDE4_ADD_EXECUTABLE (kdots
	main.cpp
	${INTERFACE_HEAD}
)

TARGET_LINK_LIBRARIES (kdotslib
	${KDOTS_REQUIRED_LIBS}
)

SET (GENERIC_LIB_VERSION "0.1.0")
SET (GENERIC_LIB_SOVERSION "1")

SET_TARGET_PROPERTIES (kdotslib
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

TARGET_LINK_LIBRARIES (kdots
	${KDOTS_REQUIRED_LIBS}
	kdotslib
)
	
IF (UNIX)
	INSTALL (TARGETS kdots DESTINATION ${BIN_INSTALL_DIR})
	INSTALL (TARGETS kdotslib DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})
	INSTALL (FILES kdotsui.rc DESTINATION ${DATA_INSTALL_DIR}/kdots)
	INSTALL (PROGRAMS kdots.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR})
ENDIF (UNIX)

# uninstall target
CONFIGURE_FILE (
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET (uninstall
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

ADD_SUBDIRECTORY (plugins)

#INCLUDE (MacroOptionalAddSubdirectory)
#MACRO_OPTIONAL_ADD_SUBDIRECTORY (po)