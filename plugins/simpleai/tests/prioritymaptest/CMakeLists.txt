INCLUDE_DIRECTORIES (
	${SIMPLEAI_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

QT4_WRAP_CPP (PRIORITYMAP_MOC prioritymaptest.hpp)

ADD_EXECUTABLE (priority_map_test
	prioritymaptest.cpp
	${PRIORITYMAP_MOC}
)
TARGET_LINK_LIBRARIES (priority_map_test
	${QT_LIBRARIES}
	kdots_simpleai
	lib_kdots
)

ADD_TEST (PriorityMapTest priority_map_test)