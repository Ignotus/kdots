cmake_minimum_required(VERSION 2.8.12)
project(kdots)

find_package(ECM REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDECompilerSettings)
include(ECMInstallIcons)
set(CMAKE_AUTOMOC ON)

find_package(Boost 1.49 REQUIRED)
find_package(KF5 REQUIRED COMPONENTS
  TextWidgets
  XmlGui
  I18n
  Completion
  KDEGames
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fexceptions")

set(PLUGIN_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/plugins)
set(KDOTS_INCLUDE_DIR
  ${CMAKE_CURRENT_BINARY_DIR}/kdotslib
  ${CMAKE_CURRENT_SOURCE_DIR}/kdotslib
)

set(PLUGINS_DIR ${CMAKE_INSTALL_PREFIX}/${KDE_INSTALL_PLUGINDIR}/kdots)

get_target_property(KF5KDEGames_INCLUDES KF5KDEGames INTERFACE_INCLUDE_DIRECTORIES)
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${KDOTS_INCLUDE_DIR}
  ${KF5KDEGames_INCLUDES}
  ${Boost_INCLUDE_DIR}
)

set(KDOTS_PLUGIN_DEPS
  ${KDEGAMES_LIBRARY}
  kdotslib
)

file(GLOB HDRS *.hpp)
file(GLOB SRCS *.cpp)
file(GLOB FORMS ui/*.ui)

ki18n_wrap_ui(SRCS ${FORMS})
kconfig_add_kcfg_files(SRCS kdots.kcfgc)
install(FILES kdots.kcfg DESTINATION ${KCFG_INSTALL_DIR})

add_executable(kdots ${SRCS})

target_link_libraries(kdots
  kdotslib
  ${KDEGAMES_LIBRARY}
  KF5::TextWidgets
  KF5::XmlGui
  KF5::Completion
)

install(TARGETS kdots DESTINATION ${BIN_INSTALL_DIR})
install(FILES kdotsui.rc DESTINATION ${KXMLGUI_INSTALL_DIR}/kdots)
install(PROGRAMS org.kde.kdots.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES cmake/KDotsLibConfig.cmake DESTINATION ${CMAKECONFIG_INSTALL_PREFIX}/KDotsLib)

add_subdirectory(kdotslib)
add_subdirectory(plugins)
add_subdirectory(doc)
add_subdirectory(pictures)
