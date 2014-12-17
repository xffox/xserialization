cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/test")

include(FindPkgConfig)
pkg_search_module(CPPUNIT cppunit)
if(CPPUNIT_FOUND)
    set(SERIALIZATION_TEST_NAME serialization_test)

    set(SERIALIZATION_TEST_SRCS
        "test.cpp"
        "MetaObjectSerializerTest.cpp"
        )

    add_executable(${SERIALIZATION_TEST_NAME} ${SERIALIZATION_TEST_SRCS})
    target_link_libraries(${SERIALIZATION_TEST_NAME} ${SERIALIZATION_LIBS}
        ${CPPUNIT_LIBRARIES})

    add_test(NAME SerializationTest COMMAND ${SERIALIZATION_TEST_NAME})
else(CPPUNIT_FOUND)
    message(WARNING "cppunit not found - testing is not available")
endif(CPPUNIT_FOUND)
