cmake_minimum_required(VERSION 2.8.12)

project(simpleai)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${KDOTS_INCLUDE_DIR}
)

set(SIMPLEAI_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB SIMPLEAI_HEADERS *.hpp)
file(GLOB SIMPLEAI_SRCS *.cpp)

add_library(kdots_simpleai MODULE
  ${SIMPLEAI_SRCS}
)

target_link_libraries(kdots_simpleai
  ${KDOTS_REQUIRED_LIBS}
  kdotslib
)

install(TARGETS kdots_simpleai DESTINATION ${KDE_INSTALL_PLUGINDIR}/kdots)
install(FILES rules DESTINATION ${KDE_INSTALL_PLUGINDIR}/kdots)