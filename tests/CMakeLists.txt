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
    set(TEST_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/test-attribute.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test-child-node.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test-encoding.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test-exception.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test-parent-node.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test-standalone.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test-version.cpp
    )

    # Enable unit tests
    enable_testing()

    # Remove C++ compiler flag
    string(REPLACE "-fno-rtti" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

    # Add possible stubs created in tests directory
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})

    # Add CPPUNIT include directory
    include_directories(${CPPUNIT_INCLUDE_DIRS})

    # Add XML include directory
    if(EXISTS ${XML_INCLUDE_DIR})
        include_directories(${XML_INCLUDE_DIR})
    endif()

    # Add library directory
    if(EXISTS ${XML_BUILD_DIR})
        link_directories(${XML_BUILD_DIR})
    endif()

    # For each unit test to create
    foreach(TEST_SOURCE_FILE ${TEST_SOURCE_FILES})

        # Get the test executable name
        get_filename_component(TEST_TARGET ${TEST_SOURCE_FILE} NAME_WE)

        # Add the unit test executable
        add_executable(${TEST_TARGET}
            ${TEST_SOURCE_FILE}
            ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        )

        # Link against CPPUNIT libraries
        target_link_libraries(${TEST_TARGET} ${CPPUNIT_LIBRARIES})

        # Link against XML library
        target_link_libraries(${TEST_TARGET} -lxml)

        # Set C++11 flag
        target_compile_features(${TEST_TARGET} PRIVATE cxx_variadic_templates)

        # Register the unit test
        add_test(NAME ${TEST_TARGET} COMMAND ${TEST_TARGET})

        # Add custom command to generate stamp file
        add_custom_command(OUTPUT ${TEST_TARGET}.stamp
            COMMAND ${TEST_TARGET}
            COMMAND ${CMAKE_COMMAND} -E touch "${TEST_TARGET}.stamp"
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            DEPENDS ${TEST_TARGET}
        )

        # Add custom target to run unit tests during build
        add_custom_target(${TEST_TARGET}.stamp ALL
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${TEST_TARGET}.stamp
        )

    endforeach()

else()
    message(WARNING "CPPUNIT not found.")
endif()
