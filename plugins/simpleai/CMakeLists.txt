CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(simpleai)

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${KDOTS_SOURCE_DIR}
)

SET(SIMPLEAI_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

IF(TESTS)
  ENABLE_TESTING()
ENDIF(TESTS)

SET(SIMPLEAI_HEADERS
  prioritymap.hpp
  rival.hpp
  plugin.hpp
)

SET(SIMPLEAI_SRCS
  prioritymap.cpp
  rival.cpp
  plugin.cpp
)

kde4_add_plugin(kdots_simpleai SHARED
  ${SIMPLEAI_SRCS}
)

TARGET_LINK_LIBRARIES(kdots_simpleai
  ${KDOTS_REQUIRED_LIBS}
  kdotslib
)

IF(UNIX)
  INSTALL(TARGETS kdots_simpleai DESTINATION ${PLUGINS_DIR})
ENDIF(UNIX)

GET_PROPERTY(kdots_simpleai_location TARGET kdots_simpleai PROPERTY LOCATION)

IF(UNIX)
  ADD_CUSTOM_COMMAND(
    TARGET kdots_simpleai
    POST_BUILD
    COMMAND cp ARGS ${kdots_simpleai_location}
    ${PLUGIN_BUILD_DIR}
  )
ELSE(UNIX)
  ADD_CUSTOM_COMMAND(
    TARGET kdots_simpleai
    POST_BUILD
    COMMAND copy ARGS ${kdots_simpleai_location}
    ${PLUGIN_BUILD_DIR}
  )
ENDIF(UNIX)