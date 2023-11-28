cmake_minimum_required(VERSION 3.5)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/test")

include(FindPkgConfig)
pkg_search_module(CPPUNIT cppunit)
if(CPPUNIT_FOUND)
    set(XSERIALIZATION_TEST_NAME xserialization_test)

    file(GLOB XSERIALIZATION_TEST_SRCS "*.cpp")

    add_executable(${XSERIALIZATION_TEST_NAME} ${XSERIALIZATION_TEST_SRCS})
    target_link_libraries(${XSERIALIZATION_TEST_NAME} ${XSERIALIZATION}
        ${CPPUNIT_LIBRARIES})

    add_test(NAME SerializationTest COMMAND ${XSERIALIZATION_TEST_NAME})
else(CPPUNIT_FOUND)
    message(WARNING "cppunit not found - testing is not available")
endif(CPPUNIT_FOUND)
