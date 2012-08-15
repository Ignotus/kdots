CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT (singlepc)

IF (COMMAND cmake_policy)
	cmake_policy (SET CMP0017 NEW)
ENDIF (COMMAND cmake_policy)

INCLUDE_DIRECTORIES (
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${KDOTS_SOURCE_DIR}
)

SET (SINGLEPC_HEADERS
	plugin.hpp
)

SET (SINGLEPC_SRCS
	plugin.cpp
)

kde4_add_plugin (kdots_singlepc
	${SINGLEPC_SRCS}
	${INTERFACES}
)

TARGET_LINK_LIBRARIES (kdots_singlepc
	${KDE4_KDECORE_LIBS}
	${KDE4_KDEUI_LIBS}
	${QT_LIBRARIES}
	kdotslib
)

IF (UNIX)
	INSTALL (TARGETS kdots_singlepc DESTINATION ${PLUGINS_DIR})
ENDIF (UNIX)

GET_PROPERTY (kdots_singlepc_location TARGET kdots_singlepc PROPERTY LOCATION)

IF (UNIX)
	ADD_CUSTOM_COMMAND(
		TARGET kdots_singlepc
		POST_BUILD
		COMMAND cp ARGS ${kdots_singlepc_location}
		${PLUGIN_BUILD_DIR}
	)
ELSE (UNIX)
	ADD_CUSTOM_COMMAND(
		TARGET kdots_singlepc
		POST_BUILD
		COMMAND copy ARGS ${kdots_singlepc_location}
		${PLUGIN_BUILD_DIR}
	)
ENDIF (UNIX)