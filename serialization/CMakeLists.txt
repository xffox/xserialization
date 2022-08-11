cmake_minimum_required(VERSION 3.0)

set(SERIALIZATION "serialization")
set(SERIALIZATION ${SERIALIZATION} PARENT_SCOPE)

set(SERIALIZATION_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB SERIALIZATION_SRCS "src/*.cpp"  "src/*/*.cpp")

add_library(${SERIALIZATION} ${SERIALIZATION_SRCS})
target_include_directories(${SERIALIZATION} PUBLIC ${SERIALIZATION_INCLUDES})

add_subdirectory("test")
