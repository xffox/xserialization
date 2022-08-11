#include <cppunit/TestAssert.h>
#include <string>
#include <utility>
#include <limits>
#include <sstream>

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include <serialization/serialization.hpp>
#include <serialization/json/json_serialization.hpp>

namespace serialization::json::test
{
    namespace
    {
        MT_CLASS(WeakData)
        {
        public:
            WeakData() = default;
            WeakData(int intVal, std::string strVal)
                :intVal(intVal), strVal(std::move(strVal))
            {}

            MT_FIELD(intVal, int,
                    MT_FIELD_ATTR_WEAK, MT_FIELD_ATTR_OPTIONAL);
            MT_FIELD(strVal, std::string,
                    MT_FIELD_ATTR_WEAK, MT_FIELD_ATTR_OPTIONAL);
        };

        bool operator==(const WeakData &left, const WeakData &right)
        {
            return left.intVal == right.intVal &&
                left.strVal == right.strVal;
        }
    }

    class JSONDeserializerTest: public CppUnit::TestCase
    {
        CPPUNIT_TEST_SUITE(JSONDeserializerTest);
        CPPUNIT_TEST(testExactFields);
        CPPUNIT_TEST(testExtraFieldsThrow);
        CPPUNIT_TEST(testMissingFieldsDefault);
        CPPUNIT_TEST(testFloatToIntegerThrow);
        CPPUNIT_TEST(testHierarchy);
        CPPUNIT_TEST_SUITE_END();
    public:
        void testExactFields()
        {
            const WeakData exp(42, "cthulhu");
            WeakData act{};
            auto &&serializer = toSerializer(act);
            std::stringstream ss;
            ss<<"{\"intVal\": "<<exp.intVal
                <<", \"strVal\": \""<<exp.strVal<<"\"}";
            serializer<<JSON(ss.str());
            CPPUNIT_ASSERT(exp == act);
        }

        void testExtraFieldsThrow()
        {
            WeakData act{};
            auto &&serializer = toSerializer(act);
            CPPUNIT_ASSERT_THROW(
                    serializer<<JSON(
                        "{\"intVal\": 42, \"strVal\": \"cthulhu\", \"otherVal\": 100}"),
                    exception::SerializerException);
        }

        void testMissingFieldsDefault()
        {
            const WeakData exp(1918, "cthulhu");
            WeakData act{};
            act.intVal = exp.intVal;
            auto &&serializer = toSerializer(act);
            std::stringstream ss;
            ss<<"{\"strVal\": \""<<exp.strVal<<"\"}";
            serializer<<JSON(ss.str());
            CPPUNIT_ASSERT(exp == act);
        }

        void testLongVal()
        {
            MT_CLASS(LongValData)
            {
            public:
                LongValData() = default;
                LongValData(unsigned long long int val)
                    :val(val)
                {}

                constexpr bool operator==(const LongValData &that) const
                {
                    return val == that.val;
                }

                MT_FIELD(val, unsigned long long int) = 0;
            };
            const LongValData exp{std::numeric_limits<decltype(exp.val)>::max()};
            LongValData act{};
            auto &&serializer = toSerializer(act);
            std::stringstream ss;
            ss<<"{\"val\": "<<exp.val<<"}";
            serializer<<JSON(ss.str());
            CPPUNIT_ASSERT(exp == act);
        }

        void testFloatToIntegerThrow()
        {
            MT_CLASS(Cl)
            {
            public:
                MT_FIELD(val, int, MT_FIELD_ATTR_WEAK) = {};
            };
            Cl act;
            auto &&serializer = toSerializer(act);
            CPPUNIT_ASSERT_THROW(serializer<<json::JSON("{\"val\": 42.43}"),
                        exception::SerializerException);
        }

        void testIntegerToFloat()
        {
            MT_CLASS(Cl)
            {
            public:
                Cl() = default;
                explicit Cl(float val)
                    :val(val)
                {}
                MT_FIELD(val, float, MT_FIELD_ATTR_WEAK) = {};

                constexpr bool operator==(const Cl &that) const
                {
                    // TODO: float comparison
                    return val == that.val;
                }
            };
            const Cl exp{42};
            Cl act;
            auto &&serializer = toSerializer(act);
            std::stringstream ss;
            ss<<"{\"val\": "<<exp.val<<"}";
            serializer<<json::JSON(ss.str());
            CPPUNIT_ASSERT(exp == act);
        }

        void testHierarchy()
        {
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
            const OuterCl exp(InnerCl(42));
            OuterCl act{};
            auto s = toSerializer(act);
            std::stringstream ss;
            ss<<"{\"obj\": {\"val\": "<<exp.obj.val<<"}}";
            s<<JSON(ss.str());
            CPPUNIT_ASSERT(act == exp);
        }
    };
    CPPUNIT_TEST_SUITE_REGISTRATION(JSONDeserializerTest);
}
