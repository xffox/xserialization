cmake_minimum_required(VERSION 2.8)

project(serialization)

include(CTest)

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

if(UNIX)
    set(STANDARD_FLAGS "-ansi")
    set(WARN_FLAGS "-Wall -Wextra -pedantic -std=c++11")
    add_definitions("${STANDARD_FLAGS}" "${WARN_FLAGS}")
elseif(WIN32)
    add_definitions("-DNOMINMAX")
endif()

add_subdirectory(serialization)
add_subdirectory(variant)
add_subdirectory(sample)
