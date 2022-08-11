cmake_minimum_required(VERSION 3.0)

set(XSERIALIZATION_JSON "xserialization_json")
set(XSERIALIZATION_JSON ${XSERIALIZATION_JSON} PARENT_SCOPE)

file(GLOB XSERIALIZATION_JSON_SRCS "src/*.cpp" "src/*/*.cpp")

set(XSERIALIZATION_JSON_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include")

find_package(nlohmann_json REQUIRED)

add_library(${XSERIALIZATION_JSON} ${XSERIALIZATION_JSON_SRCS})
target_link_libraries(${XSERIALIZATION_JSON}
    PUBLIC ${XSERIALIZATION} nlohmann_json::nlohmann_json)
target_include_directories(${XSERIALIZATION_JSON}
    PUBLIC ${XSERIALIZATION_JSON_INCLUDES})

add_subdirectory(test)
