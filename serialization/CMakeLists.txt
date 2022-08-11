cmake_minimum_required(VERSION 3.0)

set(XSERIALIZATION "xserialization")
set(XSERIALIZATION ${XSERIALIZATION} PARENT_SCOPE)

set(XSERIALIZATION_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB XSERIALIZATION_SRCS "src/*.cpp"  "src/*/*.cpp")

add_library(${XSERIALIZATION} ${XSERIALIZATION_SRCS})
target_include_directories(${XSERIALIZATION} PUBLIC ${XSERIALIZATION_INCLUDES})

add_subdirectory("test")
