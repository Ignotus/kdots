INCLUDE_DIRECTORIES (
	${SIMPLEAI_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

kde4_add_executable (priority_map_test
	prioritymaptest.cpp
)

TARGET_LINK_LIBRARIES (priority_map_test
	${KDE4_KDECORE_LIBS}
	${KDE4_KDEUI_LIBS}
	${QT_LIBRARIES}
	kdots_simpleai
	kdotslib
)

ADD_TEST (PriorityMapTest priority_map_test)