#
# Find the CppUnit includes and library
#
# This module defines
# CPPUNIT_FOUND       , If false, do not try to use CppUnit.
# CPPUNIT_INCLUDE_DIRS, where to find CppUnit header files
# CPPUNIT_LIBRARIES   , the libraries to link against to use CppUnit.

# Find include path
FIND_PATH(CPPUNIT_INCLUDE_DIR cppunit/TestCase.h
    PATHS /usr/local/include /usr/include
)

# List of possible library paths
SET(CPPUNIT_LIBRARY_PATHS
    ${CPPUNIT_INCLUDE_DIR}/../lib
    /usr/local/lib
    /usr/lib
)

IF(WIN32)
    # With Win32, important to have both
    IF(CMAKE_BUILD_TYPE == "RELEASE" || CMAKE_BUILD_TYPE == "MINSIZEREL")
        FIND_LIBRARY(CPPUNIT_LIBRARY cppunit
            PATHS ${CPPUNIT_LIBRARY_PATHS}
            PATH_SUFFIXES ${CMAKE_LIBRARY_ARCHITECTURE}
        )
    ELSE()
        FIND_LIBRARY(CPPUNIT_LIBRARY cppunitd
            PATHS ${CPPUNIT_LIBRARY_PATHS}
            PATH_SUFFIXES ${CMAKE_LIBRARY_ARCHITECTURE}
        )
    ENDIF()
ELSE(WIN32)
    # On unix system, debug and release have the same name
    FIND_LIBRARY(CPPUNIT_LIBRARY cppunit
        PATHS ${CPPUNIT_INCLUDE_DIR}/../lib /usr/local/lib /usr/lib
        PATH_SUFFIXES ${CMAKE_LIBRARY_ARCHITECTURE}
    )
ENDIF(WIN32)

# Hide temporary variables
MARK_AS_ADVANCED(CPPUNIT_INCLUDE_DIR CPPUNIT_LIBRARY)

# Set output variables
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
    SET(CPPUNIT_INCLUDE_DIRS ${CPPUNIT_INCLUDE_DIR})
    SET(CPPUNIT_LIBRARIES ${CPPUNIT_LIBRARY} ${CMAKE_DL_LIBS})
ENDIF()
