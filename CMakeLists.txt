CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT (kdots)

FIND_PACKAGE (Qt4 REQUIRED)
FIND_PACKAGE (KDE4 REQUIRED)

INCLUDE (KDE4Defaults)

OPTION (KDE4_BUILD_TESTS "Enable test units" OFF)

IF (KDE4_BUILD_TESTS)
	ENABLE_TESTING ()
	SET (QT_USE_QTTEST TRUE)
ENDIF (KDE4_BUILD_TESTS)

SET (QT_USE_QTGUI TRUE)
SET (QT_USE_QTCORE TRUE)

INCLUDE (${QT_USE_FILE})


INCLUDE_DIRECTORIES (
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${KDE4_INCLUDES}
	${QT_INCLUDES}
)

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

SET (PLUGIN_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/plugins)
SET (KDOTS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

SET (INSTALL_PREFIX "/usr/local")

IF (UNIX)
	SET (LIBDIR ${INSTALL_PREFIX}/lib${LIB_SUFFIX})
	SET (PLUGINS_DIR ${LIBDIR}/kdots/plugins)
	SET (BINDIR bin)
ENDIF (UNIX)

SET (SRCS
	graphpoint.cpp
	graph.cpp
	dottable.cpp
	stepqueue.cpp
	tablewidget.cpp
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

kde4_add_ui_files (FORMS_HEADERS ${FORMS})

CONFIGURE_FILE (config.hpp.in config.hpp)

kde4_add_library (kdotslib SHARED
	${SRCS}
)

kde4_add_executable (kdots
	main.cpp
	${FORMS_HEADERS}
	${INTERFACE_HEAD}
)

TARGET_LINK_LIBRARIES (kdotslib
	${KDE4_KDECORE_LIBS}
	${KDE4_KDEUI_LIBS}
	${QT_LIBRARIES}
)

TARGET_LINK_LIBRARIES (kdots
	${KDE4_KDECORE_LIBS}
	${KDE4_KDEUI_LIBS}
	${QT_LIBRARIES}
	kdotslib
)
	
IF (UNIX)
	INSTALL (TARGETS kdots DESTINATION ${BINDIR})
	INSTALL (TARGETS kdotslib DESTINATION ${LIBDIR})
ENDIF (UNIX)

# uninstall target
CONFIGURE_FILE (
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET (uninstall
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

ADD_SUBDIRECTORY (plugins)