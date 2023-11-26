cmake_minimum_required(VERSION 3.0)

project(xserialization)

include(CTest)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

if(UNIX)
    set(WARN_FLAGS "-Wall" "-Wextra" "-pedantic")
    add_compile_options("${WARN_FLAGS}")
elseif(WIN32)
    add_compile_options("-DNOMINMAX")
endif()

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/XSerializationConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/XSerialization")

install(EXPORT XSerializationTargets NAMESPACE xserialization::
    FILE "XSerializationConfig.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/XSerialization")
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/XSerializationConfig.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/XSerialization")

add_subdirectory(serialization)
if(USE_JSON)
    add_subdirectory(json)
endif()
if(ENABLE_SAMPLE)
    add_subdirectory(sample)
endif()
