# /project/src/CMakeLists.txt
# Martin Jabůrek
#
# Nested CMakeLists.txt to retrieve all source files from project structure.

file(GLOB SRC_HERE *.cpp)

set(SRC ${SRC}
    ${SRC_HERE}
    PARENT_SCOPE
)
