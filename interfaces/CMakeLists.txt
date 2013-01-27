project(kdotscore)

set(SRCS
  iplugin.cpp
)

set(HDRS
  iplugin.h
  pluginmacros.h
)

kde4_add_library(kdotscore SHARED ${SRCS})
 
target_link_libraries(kdotscore ${KDE4_KDEUI_LIBS})
 
install(TARGETS kdotscore ${INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES kdots_plugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})