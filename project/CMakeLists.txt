# CMakeLists.txt made using guide in README.md at https://github.com/antlr/antlr4/tree/master/runtime/Cpp/cmake and modified.

cmake_minimum_required (VERSION 2.8...3.29 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)

project(jabukod)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug) # implicitly build with debug info
endif()

file(GLOB_RECURSE HDR *.h)

set(SRC main.cpp)
add_subdirectory(src)

### ANTLR related: ###

# add needed antlr4 module
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(ExternalAntlr4Cpp)

# provide generator tool
set(ANTLR_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/../tool/antlr-4.13.2-complete.jar)

# ANTLR version is kept at latest at time of project creation.
set(ANTLR4_TAG 4.13.2)
find_package(ANTLR REQUIRED)

include_directories(${ANTLR4_INCLUDE_DIRS})

antlr_target(JabukodParser ${CMAKE_CURRENT_SOURCE_DIR}/../grammar/Jabukod.g4
    VISITOR
    LISTENER
    #PACKAGE jabukod_antlr # Specifies namespace for generated parser and lexer, if needed.
)

include_directories(${ANTLR_JabukodParser_OUTPUT_DIR})

add_definitions(-DANTLR4CPP_STATIC) # linking to a static library

###

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} ${SRC}
    ${HDR} # own headers
    ${ANTLR_JabukodParser_CXX_OUTPUTS} # ANTLR generated headers
)
target_link_libraries(${PROJECT_NAME} antlr4_static)

add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
