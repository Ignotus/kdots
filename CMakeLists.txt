cmake_minimum_required(VERSION 2.8.12)
project(kdots)

find_package(ECM REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

find_package(Qt5 REQUIRED
  Core
  Widgets
)

find_package(Boost 1.55 REQUIRED)
find_package(KF5 REQUIRED COMPONENTS
  TextWidgets
  XmlGui
  KDELibs4Support
)

find_package(KF5KDEGames 4.9.0 REQUIRED)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${KDEGAMES_INCLUDE_DIRS}
  ${QT_INCLUDES}
  ${Boost_INCLUDE_DIR}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fexceptions")

set(PLUGIN_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/plugins)
set(KDOTS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(INSTALL_PREFIX "/usr")

set(PLUGINS_DIR ${CMAKE_INSTALL_PREFIX}/${KDE_INSTALL_PLUGINDIR}/kdots)

message(STATUS "Setting plugin directory as ${PLUGINS_DIR}")

set(SRCS
  pluginloader.cpp
  pluginmanagerwidget.cpp
  newgamewidget.cpp
  newgamedialog.cpp
  mainwindow.cpp
  boardview.cpp
  pluginwidgetdelegate.cpp
  brushcombo.cpp
  brushcombodelegate.cpp
  main.cpp
  kdots.kcfgc
)

set(HEADERS
  pluginloader.hpp
  pluginmanagerwidget.hpp
  newgamedialog.hpp
  newgamewidget.hpp
  mainwindow.hpp
  boardview.hpp
  pluginwidgetdelegate.hpp
  brushcombo.hpp
  brushcombodelegate.hpp
)

set(INTERFACE_HEAD
  interface/iplugin.hpp
  interface/irival.hpp
  interface/iconfigurationwidget.hpp
)

set(FORMS
  ui/mainwindow.ui
  ui/newgamewidget.ui
  ui/newgamedialog.ui
  ui/pluginmanagerwidget.ui
  ui/boardconfigwidget.ui
)


ki18n_wrap_ui(SRCS ${FORMS})
kconfig_add_kcfg_files(SRCS kdots.kcfgc)
install(FILES kdots.kcfg DESTINATION ${KCFG_INSTALL_DIR})

add_executable(kdots ${SRCS})

target_link_libraries(kdots
  ${KDOTS_REQUIRED_LIBS}
  kdotslib
)

install(TARGETS kdots DESTINATION ${BIN_INSTALL_DIR})
install(FILES kdotsui.rc DESTINATION ${KXMLGUI_INSTALL_DIR}/kdots)
install(PROGRAMS kdots.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR})

add_subdirectory(kdotslib)
add_subdirectory(plugins)