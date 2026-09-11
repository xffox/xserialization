#include <string>
#include <unordered_map>

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include "xserialization/serialization.hpp"

namespace xserialization::test
{
    class DictSerializationTest: public CppUnit::TestCase
    {
        CPPUNIT_TEST_SUITE(DictSerializationTest);
        CPPUNIT_TEST(testDictUseSerializer);
        CPPUNIT_TEST(testDictUseDeserializer);
        CPPUNIT_TEST_SUITE_END();

    public:
        void testDictUseSerializer()
        {
            const std::unordered_map<std::string, int> expected{
                {"a", 42}, {"b", 17}};
            std::unordered_map<std::string, int> actual;
            auto serializer = xserialization::toSerializer(actual);
            serializer<<expected;
            CPPUNIT_ASSERT(actual == expected);
        }

        void testDictUseDeserializer()
        {
            const std::unordered_map<std::string, int> expected{
                {"tst", 123}, {"dst", 71}, {"abc", 5678}};
            std::unordered_map<std::string, int> actual;
            auto deserializer = xserialization::toDeserializer(expected);
            deserializer>>actual;
            CPPUNIT_ASSERT(actual == expected);
        }
    };
    CPPUNIT_TEST_SUITE_REGISTRATION(DictSerializationTest);
}
