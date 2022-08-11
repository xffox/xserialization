cmake_minimum_required(VERSION 3.0)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/test")

include(FindPkgConfig)
pkg_search_module(CPPUNIT cppunit)
if(CPPUNIT_FOUND)
    set(JSON_TEST_NAME xserialization_json_test)

    file(GLOB JSON_TEST_SRCS "*.cpp")

    add_executable(${JSON_TEST_NAME} ${JSON_TEST_SRCS})
    target_link_libraries(${JSON_TEST_NAME} 
        ${XSERIALIZATION_JSON}
        ${CPPUNIT_LIBRARIES})

    add_test(NAME JSONTest COMMAND ${JSON_TEST_NAME})
else(CPPUNIT_FOUND)
    message(WARNING "cppunit not found - testing is not available")
endif(CPPUNIT_FOUND)
