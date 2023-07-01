#include <string>
#include <utility>
#include <sstream>

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include <xserialization/serialization.hpp>
#include <xserialization/json/json_serialization.hpp>

namespace xserialization::json::test
{
    namespace
    {
        MT_CLASS(Empty)
        {};
    }

    class JSONSerializerTest: public CppUnit::TestCase
    {
        CPPUNIT_TEST_SUITE(JSONSerializerTest);
        CPPUNIT_TEST(testEmpty);
        CPPUNIT_TEST(testHierarchy);
        CPPUNIT_TEST_SUITE_END();
    public:
        void testEmpty()
        {
            JSON json;
            auto serializer = SerializationTrait<JSON>::toSerializer(json);
            serializer<<Empty{};
            CPPUNIT_ASSERT_EQUAL(std::string("{}"), static_cast<std::string>(json));
        }

        MT_CLASS(InnerCl)
        {
        public:
            InnerCl() = default;
            InnerCl(int val)
                :val(val)
            {}

            bool operator==(const InnerCl &that) const
            {
                return val == that.val;
            }

            MT_FIELD(val, int, MT_FIELD_ATTR_WEAK);
        };

        MT_CLASS(OuterCl)
        {
        public:
            OuterCl() = default;
            OuterCl(InnerCl obj)
                :obj(std::move(obj))
            {}

            bool operator==(const OuterCl &that) const
            {
                return obj == that.obj;
            }

            MT_FIELD(obj, InnerCl);
        };

        void testHierarchy()
        {
            const OuterCl src(InnerCl(42));
            std::stringstream ss;
            ss<<"{\"obj\":{\"val\":"<<src.obj.val<<"}}";
            const auto exp = std::move(ss).str();
            JSON json;
            auto s = toSerializer(json);
            s<<src;
            CPPUNIT_ASSERT_EQUAL(exp, static_cast<std::string>(json));
        }
    };
    CPPUNIT_TEST_SUITE_REGISTRATION(JSONSerializerTest);
}
