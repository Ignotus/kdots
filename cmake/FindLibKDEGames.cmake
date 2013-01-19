# - Try to find the kdegames library
# Once done this will define
#
#  KDEGAMES_FOUND - system has the kdegames library
#  KDEGAMES_INCLUDE_DIRS - a list of the relevant libkdegames include dirs. Allows the use of forwarding header, e.g. #include <KGameDifficulty>
#  KDEGAMES_INCLUDE_DIR - the kdegames include directory
#  KDEGAMES_LIBRARY - Link this to use the kdegames library
#
include(CheckLibraryExists)

if (KDEGAMES_INCLUDE_DIR AND KDEGAMES_LIBRARY)
    # in cache already
    SET(KDEGAMES_FOUND TRUE)

else (KDEGAMES_INCLUDE_DIR AND KDEGAMES_LIBRARY)

        # reset vars
        set(KDEGAMES_INCLUDE_DIRS)
        set(KDEGAMES_INCLUDE_DIR)
        set(KDEGAMES_LIBRARY)
        FIND_PATH(KDEGAMES_INCLUDE_DIR kgamedifficulty.h
                ${INCLUDE_INSTALL_DIR}
                ${KDE4_INCLUDE_DIR} 
                ${GNUWIN32_DIR}/include
        )
        set(KDEGAMES_INCLUDE_DIR ${KDEGAMES_INCLUDE_DIR})

        set(CMAKE_LIBRARY_PATH ${LIB_INSTALL_DIR})
        find_library(KDEGAMES_LIBRARY NAMES kdegames
                PATHS
                ${LIB_INSTALL_DIR}
                ${KDE4_LIB_DIR}
                ${GNUWIN32_DIR}/lib
        )
        set(KDEGAMES_LIBRARY ${KDEGAMES_LIBRARY})

        include(FindPackageHandleStandardArgs)
        FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibKDEGames DEFAULT_MSG KDEGAMES_INCLUDE_DIR KDEGAMES_LIBRARY)

        MARK_AS_ADVANCED(KDEGAMES_INCLUDE_DIR KDEGAMES_LIBRARY)
ENDIF(KDEGAMES_INCLUDE_DIR AND KDEGAMES_LIBRARY)

#The ${KDEGAMES_INCLUDE_DIR}/KDE addition is for the forwarding includes
IF(NOT KDEGAMES_INCLUDE_DIRS)
    SET(KDEGAMES_INCLUDE_DIRS ${KDEGAMES_INCLUDE_DIR} ${KDEGAMES_INCLUDE_DIR}/KDE ${KDEGAMES_INCLUDE_DIR}/KDE/KGame)
    MARK_AS_ADVANCED(KDEGAMES_INCLUDE_DIRS)
ENDIF(NOT KDEGAMES_INCLUDE_DIRS)


