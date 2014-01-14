cmake_minimum_required(VERSION 2.8)

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

    get_target_property(SERIALIZATION_TEST_PATH ${SERIALIZATION_TEST_NAME} LOCATION)
    add_test(SerializationTest "${SERIALIZATION_TEST_PATH}")
else(CPPUNIT_FOUND)
    message(WARNING "cppunit not found - testing is not available")
endif(CPPUNIT_FOUND)
