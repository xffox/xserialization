cmake_minimum_required(VERSION 3.5)

set(XSERIALIZATION "xserialization")
set(XSERIALIZATION ${XSERIALIZATION} PARENT_SCOPE)

set(XSERIALIZATION_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB XSERIALIZATION_SRCS "src/*.cpp"  "src/*/*.cpp")

add_library(${XSERIALIZATION} SHARED ${XSERIALIZATION_SRCS})
target_include_directories(${XSERIALIZATION} PUBLIC
    "$<BUILD_INTERFACE:${XSERIALIZATION_INCLUDES}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

install(TARGETS ${XSERIALIZATION} EXPORT XSerializationTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY "${XSERIALIZATION_INCLUDES}/"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_subdirectory("test")
