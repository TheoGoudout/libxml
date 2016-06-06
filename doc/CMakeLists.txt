cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

# Try to find doxygen
find_package(Doxygen)

# If doxygen exists, create documentation targets.
if(DOXYGEN_FOUND)

    # Define parameters
    option(INSTALL_HTML_DOC    "Whether a HTML documentation should be installed." OFF)
    option(INSTALL_LATEX_DOC   "Whether a PDF documentation (generated from LaTeX files) should be installed." OFF)
    option(INSTALL_RTF_DOC     "Whether a RTF documentation should be installed." OFF)
    option(INSTALL_MAN_DOC     "Whether a linux manual should be installed." ON)
    option(INSTALL_XML_DOC     "Whether a XML documentation should be installed." OFF)
    option(INSTALL_DOCBOOK_DOC "Whether a Docbook documentation should be installed." OFF)

    # Define parameters that might already be set
    if("${XML_INCLUDE_DIR}" STREQUAL "")
        set(XML_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../include" CACHE PATH "Path to XML library header files")
    endif()

    if("${XML_VERSION}" STREQUAL "")
        set(XML_VERSION "" CACHE STRING "The XML library version (needed for install)")
    endif()


    # Use parameter values
    if(INSTALL_HTML_DOC)
        set(GENERATE_HTML_DOC "YES")
        set(DOXYGEN_OUTPUT ${DOXYGEN_OUTPUT} "${CMAKE_CURRENT_BINARY_DIR}/html/index.html")
        set(DOC_TARGETS ${DOC_TARGETS} "${CMAKE_CURRENT_BINARY_DIR}/html/index.html")
        install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html/"
            DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libxml/${XML_VERSION}/html"
        )
    else()
        set(GENERATE_HTML_DOC "NO")
    endif()

    if(INSTALL_LATEX_DOC)
        set(GENERATE_LATEX_DOC "YES")
        set(DOXYGEN_OUTPUT ${DOXYGEN_OUTPUT} "${CMAKE_CURRENT_BINARY_DIR}/latex/refman.tex")
        set(DOC_TARGETS ${DOC_TARGETS} "${CMAKE_CURRENT_BINARY_DIR}/latex/refman.pdf")
        install(FILES "${CMAKE_CURRENT_BINARY_DIR}/latex/refman.pdf"
            DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libxml/"
            RENAME "libxml-userguide-${XML_VERSION}.pdf"
        )
        else()
            set(GENERATE_LATEX_DOC "NO")
    endif()

    if(INSTALL_RTF_DOC)
        set(GENERATE_RTF_DOC "YES")
        set(DOXYGEN_OUTPUT ${DOXYGEN_OUTPUT} "${CMAKE_CURRENT_BINARY_DIR}/rtf/refman.rtf")
        set(DOC_TARGETS ${DOC_TARGETS} "${CMAKE_CURRENT_BINARY_DIR}/rtf/refman.rtf")
        install(FILES "${CMAKE_CURRENT_BINARY_DIR}/rtf/refman.rtf"
            DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libxml/"
            RENAME "libxml-userguide-${XML_VERSION}.rtf"
        )
        else()
            set(GENERATE_RTF_DOC "NO")
    endif()

    if(INSTALL_MAN_DOC)
        set(GENERATE_MAN_DOC "YES")
        set(DOXYGEN_OUTPUT ${DOXYGEN_OUTPUT} "${CMAKE_CURRENT_BINARY_DIR}/man/man3/xml_basic_node.3")
        set(DOC_TARGETS ${DOC_TARGETS} "${CMAKE_CURRENT_BINARY_DIR}/man/man3/xml_basic_node.3")
        install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/man/"
            DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man"
        )
    else()
        set(GENERATE_MAN_DOC "NO")
    endif()

    if(INSTALL_XML_DOC)
        set(GENERATE_XML_DOC "YES")
        set(DOXYGEN_OUTPUT ${DOXYGEN_OUTPUT} "${CMAKE_CURRENT_BINARY_DIR}/xml/index.xml")
        set(DOC_TARGETS ${DOC_TARGETS} "${CMAKE_CURRENT_BINARY_DIR}/xml/index.xml")
        install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/xml/"
            DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libxml/${XML_VERSION}/xml"
        )
    else()
        set(GENERATE_XML_DOC "NO")
    endif()

    if(INSTALL_DOCBOOK_DOC)
        set(GENERATE_DOCBOOK_DOC "YES")
        set(DOXYGEN_OUTPUT ${DOXYGEN_OUTPUT} "${CMAKE_CURRENT_BINARY_DIR}/docbook/index.xml")
        set(DOC_TARGETS ${DOC_TARGETS} "${CMAKE_CURRENT_BINARY_DIR}/docbook/index.xml")
        install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/docbook/"
            DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libxml/${XML_VERSION}/docbook"
        )
    else()
        set(GENERATE_DOCBOOK_DOC "NO")
    endif()

    if("${XML_VERSION}" STREQUAL "")
        message(FATAL_ERROR "XML_VERSION is required (for install)")
    endif()


    # Set header list of input files
    set(DOXYGEN_INPUT_HEADERS
        ${XML_INCLUDE_DIR}/node-interface.h
        ${XML_INCLUDE_DIR}/parent-node.h
        ${XML_INCLUDE_DIR}/child-node.h
        ${XML_INCLUDE_DIR}/node.h
        ${XML_INCLUDE_DIR}/document.h
        ${XML_INCLUDE_DIR}/element.h
        ${XML_INCLUDE_DIR}/attribute.h
        ${XML_INCLUDE_DIR}/text.h
    )

    # Create doxygen configuration file
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/doxygen.config.in
        ${CMAKE_CURRENT_BINARY_DIR}/doxygen.config
    )

    # Set Doxygen input file
    set(DOXYGEN_INPUT ${CMAKE_CURRENT_BINARY_DIR}/doxygen.config)

    # Check if we need to create anything
    if (NOT ${INSTALL_HTML_DOC}  AND
        NOT ${INSTALL_LATEX_DOC} AND
        NOT ${INSTALL_RTF_DOC}   AND
        NOT ${INSTALL_MAN_DOC}   AND
        NOT ${INSTALL_XML_DOC}   AND
        NOT ${INSTALL_DOCBOOK_DOC}
    )
        message(WARNING "No documentation is to be generated.")
    else()

        # Add documentation target
        add_custom_target(doc ALL DEPENDS ${DOC_TARGETS})

        # Add rule to create doxygen documentation
        add_custom_command(
            OUTPUT ${DOXYGEN_OUTPUT}
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT} 2>&1 > doxygen.out
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            MAIN_DEPENDENCY ${DOXYGEN_INPUT}
            DEPENDS ${DOXYGEN_INPUT_HEADERS}
        )

    endif()

    # Add rule to create PDF from LaTeX
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/latex/refman.pdf
        COMMAND make 2>&1 > latex.out
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/latex
        MAIN_DEPENDENCY ${CMAKE_CURRENT_BINARY_DIR}/latex/refman.tex
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/latex/Makefile
    )

else()
    message(WARNING "Doxygen not found. Unable to build documentation.")
endif()