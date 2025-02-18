# CMakeLists.txt made using guide in README.md at https://github.com/antlr/antlr4/tree/master/runtime/Cpp/cmake and modified

cmake_minimum_required (VERSION 3.7 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)

project(Jabukod)

# Add needed antlr4 module
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(ExternalAntlr4Cpp)

# Provide generator tool
set(ANTLR_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/../tool/antlr-4.13.2-complete.jar)

# ANTLR version is kept at latest at time of project creation
set(ANTLR4_TAG 4.13.2)
find_package(ANTLR REQUIRED)

include_directories(${ANTLR4_INCLUDE_DIRS})

antlr_target(JabukodParser Jabukod.g4
    VISITOR
    LISTENER
    #PACKAGE jkParser # Specifies namespace for generated parser and lexer, if needed
)

include_directories(${ANTLR_JabukodParser_OUTPUT_DIR})

add_definitions(-DANTLR4CPP_STATIC) # linking to a static library
add_executable(main main.cpp 
    ${ANTLR_JabukodParser_CXX_OUTPUTS}
)
target_link_libraries(main antlr4_static)
