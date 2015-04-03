include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(KDOTS_REQUIRED_LIBS
  ${KDEGAMES_LIBRARY}
  ${Boost_LIBRARIES}
  KF5::TextWidgets
  KF5::XmlGui
  KF5::KDELibs4Support
)

file(GLOB KDOTSLIB_HDRS "*[^_][^p].hpp")
file(GLOB KDOTSLIB_SRCS *.cpp)

set(KDOTS_VERSION "0.5.4")
set(KDOTS_SOVERSION "1")

configure_file(config.hpp.in config.hpp)

add_library(kdotslib SHARED
  ${KDOTSLIB_SRCS}
)

target_link_libraries(kdotslib
  ${KDOTS_REQUIRED_LIBS}
)

set_target_properties(kdotslib
  PROPERTIES VERSION ${KDOTS_VERSION} SOVERSION ${KDOTS_SOVERSION}
)

install(TARGETS kdotslib DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES ${KDOTSLIB_HDRS} DESTINATION ${KDE_INSTALL_INCLUDEDIR}/kdots)
install(DIRECTORY interface/ DESTINATION ${KDE_INSTALL_INCLUDEDIR}/kdots/interface)