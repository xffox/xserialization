cmake_minimum_required(VERSION 2.8)

set(SAMPLE_NAME "sample")

set(SAMPLE_SRCS
    "src/main.cpp"
    )

include_directories(${SERIALIZATION_INCLUDES})

add_executable(${SAMPLE_NAME} ${SAMPLE_SRCS})
target_link_libraries(${SAMPLE_NAME} ${SERIALIZATION_LIBS})
