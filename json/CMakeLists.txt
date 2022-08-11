cmake_minimum_required(VERSION 3.0)

set(SERIALIZATION_JSON "json")
set(SERIALIZATION_JSON ${SERIALIZATION_JSON} PARENT_SCOPE)

file(GLOB SERIALIZATION_JSON_SRCS "src/*.cpp" "src/*/*.cpp")

set(SERIALIZATION_JSON_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include")

find_package(nlohmann_json REQUIRED)

add_library(${SERIALIZATION_JSON} ${SERIALIZATION_JSON_SRCS})
target_link_libraries(${SERIALIZATION_JSON}
    PUBLIC ${SERIALIZATION} nlohmann_json::nlohmann_json)
target_include_directories(${SERIALIZATION_JSON}
    PUBLIC ${SERIALIZATION_JSON_INCLUDES})

add_subdirectory(test)
