cmake_minimum_required(VERSION 2.6)

project(singlepc)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${KDOTS_INTERFACES}
)

set(SINGLEPC_HDRS
  plugin.h
)

set(SINGLEPC_SRCS
  plugin.cpp
)

kde4_add_plugin(kdots_singlepc SHARED
  ${SINGLEPC_SRCS}
  ${SINGLEPC_HDRS}
)

target_link_libraries(kdots_singlepc
  kdotscore
  ${QT_LIBRARIES}
  ${KDE4_KDECORE_LIBS}
)

install(TARGETS kdots_singlepc DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES kdots_singlepc.desktop DESTINATION ${SERVICES_INSTALL_DIR})
