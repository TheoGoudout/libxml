cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

# Add cmake_modules to the cmake module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../cmake_modules/")

# Try to find CPPUNIT
find_package(CPPUNIT)

# If CPPUNIT exists, create unit tests targets.
if(CPPUNIT_FOUND)

    # Define parameters that might already be set
    if("${XML_INCLUDE_DIR}" STREQUAL "")
        set(XML_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../include" CACHE PATH "Path to XML library header files")
    endif()

    if("${XML_BUILD_DIR}" STREQUAL "")
        set(XML_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../build" CACHE PATH "Path to XML library build dir")
    endif()


    # Set the list of unit tests
    set(CPPUNIT_TARGETS
    )

    # Enable unit tests
    enable_testing()

    # Remove C++ compiler flag
    string(REPLACE "-fno-rtti" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

    # Add CPPUNIT include directory
    include_directories(${CPPUNIT_INCLUDE_DIR})

    # Add XML include directory
    if(EXISTS ${XML_INCLUDE_DIR})
        include_directories(${XML_INCLUDE_DIR})
    endif()

    # Add library directory
    if(EXISTS ${XML_BUILD_DIR})
        link_directories(${XML_BUILD_DIR})
    endif()

    # For each unit test to create
    foreach(CPPUNIT_TARGET ${CPPUNIT_TARGETS})

        # Add the unit test executable
        add_executable(${CPPUNIT_TARGET}
            ${CMAKE_CURRENT_SOURCE_DIR}/${CPPUNIT_TARGET}.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        )

        # Link against CPPUNIT libraries
        target_link_libraries(${CPPUNIT_TARGET} ${CPPUNIT_LIBRARIES})

        # Link against XML library
        target_link_libraries(${CPPUNIT_TARGET} -lxml)

        # Register the unit test
        add_test(NAME ${CPPUNIT_TARGET} COMMAND ${CPPUNIT_TARGET})

        # Add custom command to create unit test timestamp
        add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${CPPUNIT_TARGET}.stamp
            COMMAND ${CMAKE_COMMAND} -E echo "Running ${CPPUNIT_TARGET} ...\n"
            COMMAND ${CPPUNIT_TARGET}
            COMMAND ${CMAKE_COMMAND} -E touch "${CPPUNIT_TARGET}.stamp"
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            MAIN_DEPENDENCY ${CMAKE_CURRENT_BINARY_DIR}/${CPPUNIT_TARGET}
        )

        # Add timestamp as target
        set(CPPUNIT_TESTS_TIMESTAMPS ${CPPUNIT_TESTS_TIMESTAMPS} "${CMAKE_CURRENT_BINARY_DIR}/${CPPUNIT_TARGET}.stamp")
    endforeach()

    # Add tests custom target
    add_custom_target(tests ALL DEPENDS ${CPPUNIT_TESTS_TIMESTAMPS})
else()
    message(WARNING "CPPUNIT not found.")
endif()
