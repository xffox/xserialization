cmake_minimum_required(VERSION 3.0)

set(SERIALIZATION_NAME "serialization")

set(SERIALIZATION_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SERIALIZATION_INCLUDES ${SERIALIZATION_INCLUDES} PARENT_SCOPE)

set(SERIALIZATION_LIBS ${SERIALIZATION_NAME})
set(SERIALIZATION_LIBS ${SERIALIZATION_LIBS} PARENT_SCOPE)

set(SERIALIZATION_SRCS
    "src/serialization.cpp"
    "src/factory.cpp"
    "src/MetaObjectSerializer.cpp"
    )

include_directories(${SERIALIZATION_INCLUDES})

add_library(${SERIALIZATION_NAME} ${SERIALIZATION_SRCS})

add_subdirectory("test")
