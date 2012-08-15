CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT (ipconnect)

IF (COMMAND cmake_policy)
	cmake_policy (SET CMP0017 NEW)
ENDIF (COMMAND cmake_policy)

INCLUDE_DIRECTORIES (
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${KDOTS_SOURCE_DIR}
)

SET (QT_USE_QTNETWORK TRUE)

INCLUDE (${QT_USE_FILE})

SET (IPCONNECT_HEADERS
	connectdialog.hpp
	plugin.hpp
	rival.hpp
	configurationwidget.hpp
)

SET (IPCONNECT_SRCS
	connectdialog.cpp
	plugin.cpp
	rival.cpp
	configurationwidget.cpp
)

SET (FORMS
	configurationwidget.ui
	connectdialog.ui
)

QT4_WRAP_UI (IPCONNECT_FORMS_HEADERS ${FORMS})
QT4_WRAP_CPP (IPCONNECT_HEADERS_MOC ${IPCONNECT_HEADERS})

ADD_LIBRARY (kdots_ipconnect SHARED
	${IPCONNECT_SRCS}
	${IPCONNECT_HEADERS_MOC}
	${IPCONNECT_FORMS_HEADERS}
)

TARGET_LINK_LIBRARIES (kdots_ipconnect
	${QT_LIBRARIES}
	kdots_plugin
)

IF (UNIX)
	INSTALL (TARGETS kdots_ipconnect DESTINATION ${PLUGINS_DIR})
ENDIF (UNIX)

GET_PROPERTY (kdots_ipconnect_location TARGET kdots_ipconnect PROPERTY LOCATION)

IF (UNIX)
	ADD_CUSTOM_COMMAND(
		TARGET kdots_ipconnect
		POST_BUILD
		COMMAND cp ARGS ${kdots_ipconnect_location}
		${PLUGIN_BUILD_DIR}
	)
ELSE (UNIX)
	ADD_CUSTOM_COMMAND(
		TARGET kdots_ipconnect
		POST_BUILD
		COMMAND copy ARGS ${kdots_ipconnect_location}
		${PLUGIN_BUILD_DIR}
	)
ENDIF (UNIX)