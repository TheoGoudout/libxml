cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

if("${XML_INCLUDE_DIR}" STREQUAL "")
	set(XML_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../include" CACHE PATH "Path to XML library header files")
endif()

if("${XML_BUILD_DIR}" STREQUAL "")
	set(XML_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../build" CACHE PATH "Path to XML library build dir")
endif()


# Define project
project(examples VERSION 0.1 LANGUAGES CXX)


# Set source files of library
add_executable(xml_format ${CMAKE_CURRENT_SOURCE_DIR}/format.cpp)

# Add dependency to XML library
if(TARGET xml)
	add_dependencies(xml_format xml)
endif()

# Set compilation flags
set(CMAKE_CXX_FLAGS_DEFAULT "-O2 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")
set(CMAKE_CXX_FLAGS "-Wall -Werror -fno-rtti" )

# Set C++11 flag
target_compile_features(xml_format PRIVATE cxx_variadic_templates)

# Add include directory
if(EXISTS ${XML_INCLUDE_DIR})
	include_directories(${XML_INCLUDE_DIR})
endif()

# Add library directory
if(EXISTS ${XML_BUILD_DIR})
	link_directories(${XML_BUILD_DIR})
endif()

# Link against XML library
target_link_libraries(xml_format -lxml)


# Install examples
install(TARGETS xml_format
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
)
