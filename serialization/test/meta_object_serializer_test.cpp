#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <string>
#include <unordered_map>
#include <limits>

#include "xserialization/serialization.hpp"

#define VALUE(cl, type) \
    MT_CLASS(cl) \
    { \
    public: \
        cl():value(){} \
        cl(const type &value):value(value){} \
        MT_FIELD(value, type); \
        bool operator==(const cl &that) const \
        { return value == that.value; } \
    }

namespace xserialization::test
{
    namespace
    {
        MT_CLASS(Point)
        {
        public:
            Point()
                :x(0), y(0)
            {}
            Point(int x, int y)
                :x(x), y(y)
            {}

            Point &operator++()
            {
                ++x;
                return *this;
            }

            Point operator++(int)
            {
                Point res(*this);
                this->operator++();
                return res;
            }

            MT_FIELD(x, int);
            MT_FIELD(y, int);
        };

        inline bool operator==(const Point &left, const Point &right)
        {
            return left.x == right.x && left.y == right.y;
        }

        MT_CLASS(Line)
        {
        public:
            Line()
                :begin(), end()
            {}
            Line(const Point &begin, const Point &end)
                :begin(begin), end(end)
            {}

            MT_FIELD(begin, Point);
            MT_FIELD(end, Point);
        };

        MT_CLASS(Vector)
        {
        public:
            typedef std::vector<int> ValueType;

        public:
            Vector()
            :v()
            {}
            Vector(const std::vector<int> &v)
            :v(v)
            {}

            MT_FIELD(v, std::vector<int>);
        };

        MT_CLASS(PointVector)
        {
        public:
            typedef std::vector<Point> ValueType;

        public:
            PointVector()
            :v()
            {}
            PointVector(const std::vector<Point> &v)
            :v(v)
            {}

            MT_FIELD(v, std::vector<Point>);
        };

        inline bool operator==(const Line &left, const Line &right)
        {
            return left.begin == right.begin && left.end == right.end;
        }

        inline bool operator==(const Vector &left, const Vector &right)
        {
            return left.v == right.v;
        }

        inline bool operator==(const PointVector &left, const PointVector &right)
        {
            return left.v == right.v;
        }

        VALUE(Bool, bool);
        VALUE(Char, char);
        VALUE(UnsignedChar, unsigned char);
        VALUE(SignedChar, signed char);
        VALUE(Short, short);
        VALUE(UnsignedShort, unsigned short);
        VALUE(Int, int);
        VALUE(UnsignedInt, unsigned int);
        VALUE(Long, long);
        VALUE(UnsignedLong, unsigned long);
        VALUE(LongLong, long long);
        VALUE(UnsignedLongLong, unsigned long long);
        VALUE(Float, float);
        VALUE(Double, double);
        VALUE(LongDouble, long double);
        VALUE(String, std::string);
    }

    namespace
    {
        template<typename T>
        T generate(const typename T::value_type &first,
            const std::size_t count)
        {
            T res;
            typename T::value_type current(first);
            for(std::size_t i = 0; i < count; ++i)
                res.push_back(current++);
            return res;
        }
    }

    namespace
    {
        MT_CLASS(SignedCharWeakClass)
        {
        public:
            SignedCharWeakClass(signed char value)
                :value(value)
            {}

            MT_FIELD(value, signed char, MT_FIELD_ATTR_WEAK);
        };
        bool operator==(const SignedCharWeakClass &left,
                const SignedCharWeakClass &right)
        {
            return left.value == right.value;
        }
        MT_CLASS(IntWeakClass)
        {
        public:
            IntWeakClass(int value)
                :value(value)
            {}

            MT_FIELD(value, int, MT_FIELD_ATTR_WEAK);
        };
        bool operator==(const IntWeakClass &left, const IntWeakClass &right)
        {
            return left.value == right.value;
        }

        MT_CLASS(SignedCharStrongClass)
        {
        public:
            SignedCharStrongClass(signed char value)
                :value(value)
            {}

            MT_FIELD(value, signed char);
        };
        MT_CLASS(IntStrongClass)
        {
        public:
            IntStrongClass(int value)
                :value(value)
            {}

            MT_FIELD(value, int);
        };
    }

    class MetaObjectSerializerTest: public CppUnit::TestCase
    {
        CPPUNIT_TEST_SUITE(MetaObjectSerializerTest);
        CPPUNIT_TEST(testSerializePlain);
        CPPUNIT_TEST(testDeserializePlain);
        CPPUNIT_TEST(testSerializeComplex);
        CPPUNIT_TEST(testDeserializeComplex);
        CPPUNIT_TEST(testSerializeCollectionPlain);
        CPPUNIT_TEST(testDeserializeCollectionPlain);
        CPPUNIT_TEST(testSerializeCollectionComplex);
        CPPUNIT_TEST(testDeserializeCollectionComplex);
        CPPUNIT_TEST(testClassName);
        CPPUNIT_TEST(testWeakFieldsShortToInt);
        CPPUNIT_TEST(testWeakFieldsIntToShort);
        CPPUNIT_TEST_EXCEPTION(testStrongFields,
                xserialization::exception::SerializationException);
        CPPUNIT_TEST_EXCEPTION(testWeakFieldsOverflow,
                xserialization::exception::SerializationException);
        CPPUNIT_TEST(testClassAllFields);
        CPPUNIT_TEST(testClassMissingFields);
        CPPUNIT_TEST(testWeakClassMissingFields);
        CPPUNIT_TEST(testExtraFieldThrow);
        CPPUNIT_TEST(testOpenClassExtraField);
        CPPUNIT_TEST_SUITE_END();
    public:
        void testSerializePlain()
        {
            CPPUNIT_ASSERT(validateSerialization(Bool(true)));
            CPPUNIT_ASSERT(validateSerialization(Char('c')));
            CPPUNIT_ASSERT(validateSerialization(UnsignedChar(127)));
            CPPUNIT_ASSERT(validateSerialization(SignedChar(64)));
            CPPUNIT_ASSERT(validateSerialization(Short(42)));
            CPPUNIT_ASSERT(validateSerialization(UnsignedShort(42)));
            CPPUNIT_ASSERT(validateSerialization(Int(42)));
            CPPUNIT_ASSERT(validateSerialization(UnsignedInt(42)));
            CPPUNIT_ASSERT(validateSerialization(Long(42)));
            CPPUNIT_ASSERT(validateSerialization(UnsignedLong(42)));
            CPPUNIT_ASSERT(validateSerialization(LongLong(42)));
            CPPUNIT_ASSERT(validateSerialization(UnsignedLongLong(42)));
            CPPUNIT_ASSERT(validateSerialization(Float(42)));
            CPPUNIT_ASSERT(validateSerialization(Double(42)));
            CPPUNIT_ASSERT(validateSerialization(LongDouble(42)));
            CPPUNIT_ASSERT(validateSerialization(String("foobar")));
        }

        void testDeserializePlain()
        {
            CPPUNIT_ASSERT(validateDeserialization(Bool(true)));
            CPPUNIT_ASSERT(validateDeserialization(Char('c')));
            CPPUNIT_ASSERT(validateDeserialization(UnsignedChar(127)));
            CPPUNIT_ASSERT(validateDeserialization(SignedChar(64)));
            CPPUNIT_ASSERT(validateDeserialization(Short(42)));
            CPPUNIT_ASSERT(validateDeserialization(UnsignedShort(42)));
            CPPUNIT_ASSERT(validateDeserialization(Int(42)));
            CPPUNIT_ASSERT(validateDeserialization(UnsignedInt(42)));
            CPPUNIT_ASSERT(validateDeserialization(Long(42)));
            CPPUNIT_ASSERT(validateDeserialization(UnsignedLong(42)));
            CPPUNIT_ASSERT(validateDeserialization(LongLong(42)));
            CPPUNIT_ASSERT(validateDeserialization(UnsignedLongLong(42)));
            CPPUNIT_ASSERT(validateDeserialization(Float(42)));
            CPPUNIT_ASSERT(validateDeserialization(Double(42)));
            CPPUNIT_ASSERT(validateDeserialization(LongDouble(42)));
            CPPUNIT_ASSERT(validateDeserialization(String("foobar")));
        }

        void testSerializeComplex()
        {
            CPPUNIT_ASSERT(validateSerialization(Line()));
            CPPUNIT_ASSERT(validateSerialization(Line(Point(1, 2),
                        Point(3, 4))));
        }

        void testDeserializeComplex()
        {
            CPPUNIT_ASSERT(validateDeserialization(Line()));
            CPPUNIT_ASSERT(validateDeserialization(Line(Point(1, 2),
                        Point(3, 4))));
        }

        void testSerializeCollectionPlain()
        {
            CPPUNIT_ASSERT(validateSerialization(Vector()));
            CPPUNIT_ASSERT(validateSerialization(
                    Vector(generate<Vector::ValueType>(42, 1))));
            CPPUNIT_ASSERT(validateSerialization(
                    Vector(generate<Vector::ValueType>(42, 10))));
        }

        void testDeserializeCollectionPlain()
        {
            CPPUNIT_ASSERT(validateDeserialization(Vector()));
            CPPUNIT_ASSERT(validateDeserialization(
                    Vector(generate<Vector::ValueType>(42, 1))));
            CPPUNIT_ASSERT(validateDeserialization(
                    Vector(generate<Vector::ValueType>(42, 10))));
        }

        void testSerializeCollectionComplex()
        {
            CPPUNIT_ASSERT(validateSerialization(PointVector()));
            CPPUNIT_ASSERT(validateSerialization(PointVector(
                        generate<PointVector::ValueType>(Point(), 1))));
            CPPUNIT_ASSERT(validateSerialization(PointVector(
                        generate<PointVector::ValueType>(Point(), 10))));
        }

        void testDeserializeCollectionComplex()
        {
            CPPUNIT_ASSERT(validateDeserialization(PointVector()));
            CPPUNIT_ASSERT(validateDeserialization(PointVector(
                        generate<PointVector::ValueType>(Point(), 1))));
            CPPUNIT_ASSERT(validateDeserialization(PointVector(
                        generate<PointVector::ValueType>(Point(), 10))));
        }

        void testClassName()
        {
            CPPUNIT_ASSERT_EQUAL(std::string("Point"),
                    Point().className());
            CPPUNIT_ASSERT_EQUAL(std::string("Line"),
                    Line().className());
            CPPUNIT_ASSERT_EQUAL(std::string("Vector"),
                    Vector().className());
        }

        void testWeakFieldsShortToInt()
        {
            const SignedCharWeakClass charValue{42};
            IntWeakClass intValue{0};
            xserialization::toDeserializer(charValue)>>intValue;
            CPPUNIT_ASSERT(IntWeakClass{42} == intValue);
        }

        void testWeakFieldsIntToShort()
        {
            const IntWeakClass intValue{42};
            SignedCharWeakClass charValue{0};
            xserialization::toDeserializer(intValue)>>charValue;
            CPPUNIT_ASSERT(SignedCharWeakClass{42} == charValue);
        }

        void testStrongFields()
        {
            const SignedCharStrongClass charValue{42};
            IntStrongClass intValue{0};
            xserialization::DeserializationTrait<SignedCharStrongClass>::toDeserializer(charValue)>>intValue;
        }

        void testWeakFieldsOverflow()
        {
            if constexpr(std::numeric_limits<signed char>::max() < std::numeric_limits<int>::max())
            {
                IntWeakClass intValue{std::numeric_limits<int>::max()};
                SignedCharWeakClass charValue{0};
                xserialization::DeserializationTrait<IntWeakClass>::toDeserializer(intValue)>>charValue;
            }
            else
            {
                // not possible to test on this platform
                throw exception::SerializerException(Context());
            }
        }

        void testClassAllFields()
        {
            const FieldCl exp(42, 43, 84, 101);
            std::unordered_map<std::string, int> src{
                {"a", exp.a},
                {"b", exp.b},
                {"c", exp.c},
                {"d", exp.d},
            };
            FieldCl act{};
            auto s = toSerializer(act);
            s<<src;
            CPPUNIT_ASSERT(exp == act);
        }

        void testClassMissingFields()
        {
            std::unordered_map<std::string, int> src{
                {"b", 42},
                {"c", 101},
            };
            FieldCl act{};
            auto s = toSerializer(act);
            CPPUNIT_ASSERT_THROW(s<<src, exception::SerializerException);
        }

        void testWeakClassMissingFields()
        {
            const FieldWeakCl exp(42, 43, 131, 1001);
            std::unordered_map<std::string, int> src{
                {"a", exp.a},
                {"b", exp.b},
            };
            FieldWeakCl act{};
            act.c = exp.c;
            act.d = exp.d;
            auto s = toSerializer(act);
            s<<src;
            CPPUNIT_ASSERT(exp == act);
        }

        void testExtraFieldThrow()
        {
            MT_CLASS(Cl)
            {
            public:
                MT_FIELD(a, int, MT_FIELD_ATTR_WEAK);
                MT_FIELD(b, int, MT_FIELD_ATTR_WEAK);
            };
            Cl act{};
            const std::unordered_map<std::string, int> src{
                {"a", 42},
                {"b", 888},
                {"c", 101},
            };
            auto s = toSerializer(act);
            CPPUNIT_ASSERT_THROW(s<<src, exception::SerializerException);
        }

        void testOpenClassExtraField()
        {
            MT_CLASS(Cl, MT_CLASS_ATTR_OPEN)
            {
            public:
                Cl() = default;
                Cl(int a, int b)
                    :a(a), b(b)
                {}
                MT_FIELD(a, int, MT_FIELD_ATTR_WEAK);
                MT_FIELD(b, int, MT_FIELD_ATTR_WEAK);

                bool operator==(const Cl &that) const
                {
                    return a == that.a && b == that.b;
                }
            };
            const Cl exp{42, 888};
            const std::unordered_map<std::string, int> src{
                {"a", 42},
                {"b", 888},
                {"c", 101},
           };
            Cl act{};
            auto s = toSerializer(act);
            s<<src;
            CPPUNIT_ASSERT(exp == act);
        }

    private:
        MT_CLASS(FieldCl)
        {
        public:
            FieldCl() = default;
            FieldCl(int a, int b, int c, int d)
                :a(a), b(b), c(c), d(d)
            {}
            MT_FIELD(a, int);
            MT_FIELD(b, int);
            MT_FIELD(c, int);
            MT_FIELD(d, int);

            bool operator==(const FieldCl &that) const
            {
                return a == that.a && b == that.b && c == that.c && d == that.d;
            }
        };

        MT_CLASS(FieldWeakCl)
        {
        public:
            FieldWeakCl() = default;
            FieldWeakCl(int a, int b, int c, int d)
                :a(a), b(b), c(c), d(d)
            {}
            MT_FIELD(a, int, MT_FIELD_ATTR_OPTIONAL);
            MT_FIELD(b, int, MT_FIELD_ATTR_OPTIONAL);
            MT_FIELD(c, int, MT_FIELD_ATTR_OPTIONAL);
            MT_FIELD(d, int, MT_FIELD_ATTR_OPTIONAL);

            bool operator==(const FieldWeakCl &that) const
            {
                return a == that.a && b == that.b && c == that.c && d == that.d;
            }
        };

        template<typename T>
        bool validateSerialization(const T &value)
        {
            T actual;
            auto serializer = xserialization::toSerializer(actual);
            serializer<<value;
            return value == actual;
        }

        template<typename T>
        bool validateDeserialization(const T &value)
        {
            T v(value);
            const T expected(v);
            T actual;
            xserialization::toDeserializer(v)>>actual;
            return expected == actual;
        }
    };
    CPPUNIT_TEST_SUITE_REGISTRATION(MetaObjectSerializerTest);
}
