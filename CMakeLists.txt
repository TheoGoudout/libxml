cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

# Define parameters
option(BUILD_SHARED_LIBRARY "Whether the shared library should be built." ON)
option(BUILD_STATIC_LIBRARY "Whether the static library should be built." OFF)

# Define project
project(libxml VERSION 0.1 LANGUAGES CXX)

# Set source files of the project
set(XML_SOURCE_FILES
    src/attribute.cpp
    src/child-node.cpp
    src/document.cpp
    src/element.cpp
    src/encoding.cpp
    src/node.cpp
    src/parent-node.cpp
    src/readable.cpp
    src/standalone.cpp
    src/text.cpp
    src/version.cpp
    src/writeable.cpp
)

# Set header files of the project
set(XML_HEADER_FILES
    include/attribute.h
    include/child-node.h
    include/convert.h
    include/document.h
    include/element.h
    include/exception.h
    include/iterator.h
    include/node-interface.h
    include/node.h
    include/parent-node.h
    include/readable.h
    include/standalone.h
    include/strings.h
    include/text.h
    include/version.h
    include/writeable.h
)


# Set include directory of library
include_directories(include)

# Set compilation flags
set(CMAKE_CXX_FLAGS_DEFAULT "-O2 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")
set(CMAKE_CXX_FLAGS "-Wall -Werror -fno-rtti" )

# Register dynamic library
if(BUILD_SHARED_LIBRARY)
    # Add source files to library
    add_library(xml SHARED ${XML_SOURCE_FILES})

    # Set C++11 flag
    target_compile_features(xml PRIVATE cxx_variable_templates)

    # Set targets properties
    set_target_properties(xml PROPERTIES
        VERSION ${libxml_VERSION}
        SOVERSION ${libxml_VERSION}
    )

    # Install library
    install(TARGETS xml
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    )

endif()

# Register static library
if(BUILD_STATIC_LIBRARY)
    # Add source files to library
    add_library(xml_static STATIC ${XML_SOURCE_FILES})

    # Set C++11 flag
    target_compile_features(xml_static PRIVATE cxx_variable_templates)

    # Set targets properties
    set_target_properties(xml_static PROPERTIES
        VERSION ${libxml_VERSION}
    )

    # Install library
    install(TARGETS xml_static
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    )

endif()

if(BUILD_SHARED_LIBRARY OR BUILD_STATIC_LIBRARY)
    # Install headers
    install(FILES ${XML_HEADER_FILES}
        DESTINATION "${CMAKE_INSTALL_PREFIX}/include/xml/${libxml_VERSION}"
    )
endif()


# Set useful environment variables
set(XML_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(XML_BUILD_DIR   ${CMAKE_CURRENT_BINARY_DIR})
set(XML_VERSION     ${libxml_VERSION})

# Compile unit tests if requested
option(UNIT_TESTS "Compile and run unit tests." OFF)
if(UNIT_TESTS)
    add_subdirectory(tests/ ${CMAKE_CURRENT_SOURCE_DIR}/tests/build/)
endif()

# Compile examples if requested
option(EXAMPLES "Compile and install examples." ON)
if(EXAMPLES)
    add_subdirectory(examples/ ${CMAKE_CURRENT_SOURCE_DIR}/examples/build/)
endif()

# Generate doxygen documentation if requested
option(DOCUMENTATION "Generate HTML and PDF documentation." ON)
if(DOCUMENTATION)
	add_subdirectory(doc/ ${CMAKE_CURRENT_SOURCE_DIR}/doc/build/)
endif()