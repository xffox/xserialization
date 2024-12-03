#include <vector>

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include "xserialization/serialization.hpp"

namespace xserialization::test
{
    class CollectionSerializationTest: public CppUnit::TestCase
    {
        CPPUNIT_TEST_SUITE(CollectionSerializationTest);
        CPPUNIT_TEST(testBoolVector);
        CPPUNIT_TEST_SUITE_END();

    public:
        void testBoolVector()
        {
            const std::vector<bool> expected{true, false, false, true};
            std::vector<bool> actual;
            auto serializer = xserialization::toSerializer(actual);
            serializer<<expected;
            CPPUNIT_ASSERT(actual == expected);
        }
    };
    CPPUNIT_TEST_SUITE_REGISTRATION(CollectionSerializationTest);
}
