cmake_minimum_required(VERSION 2.8.12)

project(ipconnect)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${KDOTS_INCLUDE_DIR}
)

find_package(Qt5 REQUIRED
  Network
)


SET(IPCONNECT_HEADERS
  connectdialog.hpp
  plugin.hpp
  rival.hpp
  configurationwidget.hpp
)

SET(IPCONNECT_SRCS
  connectdialog.cpp
  plugin.cpp
  rival.cpp
  configurationwidget.cpp
)

SET(FORMS
  configurationwidget.ui
  connectdialog.ui
)

ki18n_wrap_ui(IPCONNECT_SRCS ${FORMS})

add_library(kdots_ipconnect MODULE
  ${IPCONNECT_SRCS}
)

target_link_libraries(kdots_ipconnect
  ${KDOTS_REQUIRED_LIBS}
  ${QT_LIBRARIES}
  kdotslib
)

install(TARGETS kdots_ipconnect DESTINATION ${KDE_INSTALL_PLUGINDIR}/kdots)