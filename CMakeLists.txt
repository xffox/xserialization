cmake_minimum_required(VERSION 3.0)

project(serialization)

include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

if(UNIX)
    set(WARN_FLAGS "-Wall" "-Wextra" "-pedantic")
    add_compile_options("${WARN_FLAGS}")
elseif(WIN32)
    add_compile_options("-DNOMINMAX")
endif()

add_subdirectory(serialization)
add_subdirectory(json)
add_subdirectory(variant)
add_subdirectory(sample)
