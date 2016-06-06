#
# Find the CppUnit includes and library
#
# This module defines
# CPPUNIT_INCLUDE_DIR, where to find tiff.h, etc.
# CPPUNIT_LIBRARIES, the libraries to link against to use CppUnit.
# CPPUNIT_FOUND, If false, do not try to use CppUnit.

# also defined, but not for general use are
# CPPUNIT_LIBRARY, where to find the CppUnit library.
# CPPUNIT_DEBUG_LIBRARY, where to find the CppUnit library in debug
# mode.

FIND_PATH(CPPUNIT_INCLUDE_DIR cppunit/TestCase.h /usr/local/include /usr/include)

SET(CPPUNIT_LIBRARY_PATHS
    ${CPPUNIT_INCLUDE_DIR}/../lib
    /usr/local/lib
    /usr/lib
)

IF(WIN32)
    # With Win32, important to have both
    FIND_LIBRARY(CPPUNIT_LIBRARY cppunit
        PATHS ${CPPUNIT_LIBRARY_PATHS}
        PATH_SUFFIXES ${CMAKE_LIBRARY_ARCHITECTURE}
    )

    FIND_LIBRARY(CPPUNIT_DEBUG_LIBRARY cppunitd
        PATHS ${CPPUNIT_LIBRARY_PATHS}
        PATH_SUFFIXES ${CMAKE_LIBRARY_ARCHITECTURE}
    )

ELSE(WIN32)
    # On unix system, debug and release have the same name
    FIND_LIBRARY(CPPUNIT_LIBRARY cppunit
        PATHS ${CPPUNIT_LIBRARY_PATHS}
        PATH_SUFFIXES ${CMAKE_LIBRARY_ARCHITECTURE}
    )

    FIND_LIBRARY(CPPUNIT_DEBUG_LIBRARY cppunit
        PATHS ${CPPUNIT_LIBRARY_PATHS}
        PATH_SUFFIXES ${CMAKE_LIBRARY_ARCHITECTURE}
    )

ENDIF(WIN32)

IF(NOT CPPUNIT_INCLUDE_DIR)
    SET(CPPUNIT_FOUND "NO")
    IF (CPPUNIT_FIND_REQUIRED)
        MESSAGE(SEND_ERROR "Could not find CppUnit include directory.")
    ENDIF(CPPUNIT_FIND_REQUIRED)
ELSEIF(NOT CPPUNIT_LIBRARY)
    SET(CPPUNIT_FOUND "NO")
    IF (CPPUNIT_FIND_REQUIRED)
        MESSAGE(SEND_ERROR "Could not find CppUnit library.")
    ENDIF(CPPUNIT_FIND_REQUIRED)
ELSE()
    SET(CPPUNIT_FOUND "YES")
    SET(CPPUNIT_LIBRARIES ${CPPUNIT_LIBRARY} ${CMAKE_DL_LIBS})
    SET(CPPUNIT_DEBUG_LIBRARIES ${CPPUNIT_DEBUG_LIBRARY} ${CMAKE_DL_LIBS})
ENDIF()
