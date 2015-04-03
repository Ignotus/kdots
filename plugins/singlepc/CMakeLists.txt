cmake_minimum_required(VERSION 2.8.12)

project(singlepc)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${KDOTS_INCLUDE_DIR}
)

set(SINGLEPC_HEADERS
  plugin.hpp
)

set(SINGLEPC_SRCS
  plugin.cpp
)

add_library(kdots_singlepc MODULE
  ${SINGLEPC_SRCS}
)

target_link_libraries(kdots_singlepc
  ${KDOTS_REQUIRED_LIBS}
  kdotslib
)

install(TARGETS kdots_singlepc DESTINATION ${KDE_INSTALL_PLUGINDIR}/kdots)
