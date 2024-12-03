#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include "xserialization/serialization.hpp"

namespace xserialization::test
{
class AtomSerializationTest: public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE(AtomSerializationTest);
    CPPUNIT_TEST(testAtomDeserializer);
    CPPUNIT_TEST(testAtomSerializer);
    CPPUNIT_TEST_SUITE_END();

    template<typename T>
    void checkForDeserializer(T value)
    {
        T exp = value;
        T act{};
        xserialization::toDeserializer(exp)>>act;
        CPPUNIT_ASSERT(act == exp);
    }

    template<typename T>
    void checkForSerializer(T value)
    {
        T exp = value;
        T act{};
        auto serializer = xserialization::toSerializer(act);
        serializer<<exp;
        CPPUNIT_ASSERT(act == exp);
    }

public:
    void testAtomDeserializer()
    {
        checkForDeserializer<int>(42);
        checkForDeserializer<double>(42.13);
        checkForDeserializer<std::string>("Hastur");
    }
    void testAtomSerializer()
    {
        checkForSerializer<int>(83);
        checkForSerializer<double>(117.321);
        checkForSerializer<std::string>("Nyogtha");
    }
};
CPPUNIT_TEST_SUITE_REGISTRATION(AtomSerializationTest);
}
