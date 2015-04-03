
set(KDOTS_REQUIRED_LIBS
  ${KDEGAMES_LIBRARY}
  ${QT_LIBRARIES}
  ${Boost_LIBRARIES}
  KF5::TextWidgets
  KF5::XmlGui
  KF5::KDELibs4Support
)

set(KDOTSLIB_SRCS
  point.cpp
  graphpoint.cpp
  graph.cpp
  boardmodel.cpp
  stepqueue.cpp
  polygon.cpp
  polygonfinder.cpp
  gameconfig.cpp
)

set(KDOTSLIB_HEADERS
  graphpoint.hpp
  graph.hpp
  boardmodel.hpp
  stepqueue.hpp
  edgelist.hpp
  point.hpp
  constants.hpp
  polygon.hpp
  polygonfinder.hpp
  gameconfig.hpp
  iboardview.hpp
)

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