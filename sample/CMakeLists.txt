cmake_minimum_required(VERSION 3.0)

set(SAMPLE_NAME "sample")

set(SAMPLE_SRCS
    "src/main.cpp"
    )

add_executable(${SAMPLE_NAME} ${SAMPLE_SRCS})
target_link_libraries(${SAMPLE_NAME} ${XSERIALIZATION} ${XSERIALIZATION_JSON})
