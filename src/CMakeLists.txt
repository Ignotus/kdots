CMAKE_MINIMUM_REQUIRED (VERSION 2.6)


SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
PROJECT (kdots)

FIND_PACKAGE (Qt4 REQUIRED)


SET (QT_USE_QTGUI TRUE)

INCLUDE (${QT_USE_FILE})
INCLUDE_DIRECTORIES (
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (SRCS
	stepqueue.cpp
	tablewidget.cpp
	mainwindow.cpp
	newgamedialog.cpp
	polygon.cpp
	dottable.cpp
	main.cpp
	)

SET (HEADERS
	stepqueue.h
	tablewidget.h
	mainwindow.h
	newgamedialog.h
	polygon.h
	dottable.h
	)

SET (FORMS
        mainwindow.ui
        newgamedialog.ui
 )


QT4_WRAP_CPP (HEADERS_MOC ${HEADERS})
QT4_WRAP_UI (FORMS_HEADERS ${FORMS})

ADD_EXECUTABLE (kdots ${SRCS} ${HEADERS_MOC} ${FORMS_HEADERS})
TARGET_LINK_LIBRARIES (kdots
	${QT_LIBRARIES}
)

