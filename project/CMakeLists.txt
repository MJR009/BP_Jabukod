# /project/CMakeLists.txt
# Martin Jabůrek
#
# Made using guide in README.md at https://github.com/antlr/antlr4/tree/master/runtime/Cpp/cmake and modified.

cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)

project(jabukod)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug) # implicitly build with debug info
endif()

file(GLOB_RECURSE HDR *.h)

file(GLOB_RECURSE SRC *.cpp)
list(PREPEND SRC main.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}) # common.h
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/dev)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/SymbolTable)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/SymbolTable/Value)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/AST)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/AST/NodeData)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/AST/NodeData/SpecificNodeData)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Generation)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Generation/Assembly)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Generation/GenerationMethods)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/ErrorListeners)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Obfuscation)



### ANTLR related: ###

set(GRAMMAR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../grammar/Jabukod.g4)

# add needed antlr4 module
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(ExternalAntlr4Cpp)

# provide generator tool
set(ANTLR_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/../tool/antlr-4.13.2-complete.jar)

# ANTLR version is kept at latest at time of project creation.
set(ANTLR4_TAG 4.13.2)
find_package(ANTLR REQUIRED)

include_directories(${ANTLR4_INCLUDE_DIRS})

antlr_target(JabukodParser ${GRAMMAR_PATH}
    VISITOR
    LISTENER
    #PACKAGE jabukod_antlr # Specifies namespace for generated parser and lexer, if needed.
)

include_directories(${ANTLR_JabukodParser_OUTPUT_DIR})

add_definitions(-DANTLR4CPP_STATIC) # linking to a static library

###



add_executable(${PROJECT_NAME} ${SRC}
    ${HDR} # own headers
    ${ANTLR_JabukodParser_CXX_OUTPUTS} # ANTLR generated headers
)
target_link_libraries(${PROJECT_NAME} antlr4_static)



# Fundamental run target without arguments
add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Java parse tree visualiser (grun)
set(VISUALISE_BUILD_DIR ${CMAKE_BINARY_DIR}/visualise_build)
get_filename_component(GRAMMAR_NAME ${GRAMMAR_PATH} NAME_WE)
set(STARTING_NONTERMINAL sourceFile)
add_custom_target(visualise
    COMMAND ${CMAKE_COMMAND} -E make_directory ${VISUALISE_BUILD_DIR}
    COMMAND java -jar ${ANTLR_EXECUTABLE} -o ${VISUALISE_BUILD_DIR} ${GRAMMAR_PATH}
    COMMAND javac -cp ${ANTLR_EXECUTABLE} ${VISUALISE_BUILD_DIR}/*.java
    COMMAND
        test -f ${CMAKE_CURRENT_SOURCE_DIR}/../samples/$(INPUT_FILE)
        &&
            java
                -Dorg.antlr.v4.runtime.atn.LL_EXACT_AMBIG_DETECTION=true # report all ambiguities
                -cp ${ANTLR_EXECUTABLE}:${VISUALISE_BUILD_DIR} org.antlr.v4.gui.TestRig
                ${GRAMMAR_NAME} ${STARTING_NONTERMINAL} -gui -diagnostics ${CMAKE_CURRENT_SOURCE_DIR}/../samples/$(INPUT_FILE)
        || (
            ${CMAKE_COMMAND} -E echo "" ; &&
            ${CMAKE_COMMAND} -E echo "Input file not provided or invalid" ; &&
            ${CMAKE_COMMAND} -E echo "Usage: make visualise INPUT_FILE=[file in __samples__ subdirectory]" ; &&
            ${CMAKE_COMMAND} -E echo ""
        )
)

# Creates a standalone zip archive of the project
set(ARCHIVE_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
file(GLOB PACK_FILES
    RELATIVE ${ARCHIVE_BASE_DIR}
    ${ARCHIVE_BASE_DIR}/docs/Doxyfile
    ${ARCHIVE_BASE_DIR}/docs/images/*
    ${ARCHIVE_BASE_DIR}/grammar/Jabukod.g4
    ${ARCHIVE_BASE_DIR}/grammar/Jabukod.dox
    ${ARCHIVE_BASE_DIR}/profiling/*
    ${ARCHIVE_BASE_DIR}/project/*
    ${ARCHIVE_BASE_DIR}/samples/*
    ${ARCHIVE_BASE_DIR}/test_programs/*
    ${ARCHIVE_BASE_DIR}/tool/*
    ${ARCHIVE_BASE_DIR}/README.md
    ${ARCHIVE_BASE_DIR}/assignment.pdf
    ${ARCHIVE_BASE_DIR}/Jabukod_syntax-highlighting/images/*
    ${ARCHIVE_BASE_DIR}/Jabukod_syntax-highlighting/syntaxes/*
    ${ARCHIVE_BASE_DIR}/Jabukod_syntax-highlighting/language-configuration.json
    ${ARCHIVE_BASE_DIR}/Jabukod_syntax-highlighting/package.json
    ${ARCHIVE_BASE_DIR}/Jabukod_syntax-highlighting/README.md
)
add_custom_target(pack
    COMMAND ${CMAKE_COMMAND} -E tar cfv ${PROJECT_NAME}.zip --format=zip ${PACK_FILES}
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "!!!!!! DO NOT FORGET THE SYNTAX HIGHLIGHTING FOR HANDIN !!!!!!"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    WORKING_DIRECTORY ${ARCHIVE_BASE_DIR}
)

find_package(Doxygen)

if(DOXYGEN_FOUND)
    set (DOCS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../docs)
    set (DOXYFILE ${DOCS_DIR}/Doxyfile)

    # Generate documentation
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
        COMMAND make -C ${DOCS_DIR}/latex
        COMMAND cp ${DOCS_DIR}/latex/refman.pdf ${CMAKE_BINARY_DIR}/xjabur02_Jabukod_documentation.pdf
        WORKING_DIRECTORY ${DOCS_DIR}
    )

    # Clean up generated documentation
    add_custom_target(clean_docs
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${DOCS_DIR}/html ${DOCS_DIR}/latex
    )
else()
    message(STATUS "Doxygen appears to not be installed, `make docs` will not be available.")
endif()
