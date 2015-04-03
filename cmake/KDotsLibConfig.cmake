# Once done this will define
#
#  KDOTSLIB_FOUND - system has KDotsLib
#  KDOTSLIB_INCLUDE_DIRS - the KDotsLib include directorires
#  KDOTSLIB_LIBRARIES - Link these to use KDotsLib
#
#  Copyright (C) 2015  Minh Ngo <minh@fedoraproject.org>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

if(KDOTSLIB_LIBRARIES AND KDOTSLIB_INCLUDE_DIRS)
    set(KDOTSLIB_FOUND TRUE)
else(KDOTSLIB_LIBRARIES AND KDOTSLIB_INCLUDE_DIRS)

    find_path (KDOTSLIB_INCLUDE_DIR
        NAMES
        kdots/kdots_api.hpp
        PATHS
        /include
        /usr/include
        /usr/local/include
        /opt/local/include
        /sw/include
    )

    find_library (KDOTSLIB_LIBRARY
        NAMES
        kdotslib
        PATHS
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        /sw/lib
    )

    set (KDOTSLIB_INCLUDE_DIRS
        ${KDOTSLIB_INCLUDE_DIR}/kdots
    )
    set (KDOTSLIB_LIBRARIES
        ${KDOTSLIB_LIBRARY}
    )

    if(KDOTSLIB_INCLUDE_DIRS AND KDOTSLIB_LIBRARIES)
        set(KDOTSLIB_FOUND TRUE)
    endif(KDOTSLIB_INCLUDE_DIRS AND KDOTSLIB_LIBRARIES)

    if(KDOTSLIB_FOUND)
        if(NOT KDOTSLIB_FIND_QUIETLY)
            message(STATUS "Found KDotsLib: ${KDOTSLIB_LIBRARIES} ${KDOTSLIB_INCLUDE_DIRS}")
        endif(NOT KDOTSLIB_FIND_QUIETLY)
    else(KDOTSLIB_FOUND)
        if(KDOTSLIB_FIND_REQUIRED)
            message(FATAL_ERROR "Could not find KDotsLib")
        endif(KDOTSLIB_FIND_REQUIRED)
    endif(KDOTSLIB_FOUND)

    # show the KDOTSLIB_INCLUDE_DIRS and KDOTSLIB_LIBRARIES variables only in the advanced view
    mark_as_advanced(KDOTSLIB_INCLUDE_DIRS KDOTSLIB_LIBRARIES)

endif(KDOTSLIB_LIBRARIES AND KDOTSLIB_INCLUDE_DIRS)