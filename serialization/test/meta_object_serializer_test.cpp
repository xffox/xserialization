#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <string>
#include <unordered_map>
#include <limits>

#include "serialization/serialization.hpp"

#define VALUE(cl, type) \
    SERIALIZABLE_CLASS(cl) \
    { \
    public: \
        cl():value(){} \
        cl(const type &value):value(value){} \
        SERIALIZABLE_FIELD(type, value); \
        bool operator==(const cl &that) const \
        { return value == that.value; } \
    }

namespace serialization::test
{
    namespace
    {
        SERIALIZABLE_CLASS(Point)
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

            SERIALIZABLE_FIELD(int, x);
            SERIALIZABLE_FIELD(int, y);
        };

        inline bool operator==(const Point &left, const Point &right)
        {
            return left.x == right.x && left.y == right.y;
        }

        SERIALIZABLE_CLASS(Line)
        {
        public:
            Line()
                :begin(), end()
            {}
            Line(const Point &begin, const Point &end)
                :begin(begin), end(end)
            {}

            SERIALIZABLE_FIELD(Point, begin);
            SERIALIZABLE_FIELD(Point, end);
        };

        SERIALIZABLE_CLASS(Vector)
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

            SERIALIZABLE_FIELD(std::vector<int>, v);
        };

        SERIALIZABLE_CLASS(PointVector)
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

            SERIALIZABLE_FIELD(std::vector<Point>, v);
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
        SERIALIZABLE_CLASS(SignedCharWeakClass)
        {
        public:
            SignedCharWeakClass(signed char value)
                :value(value)
            {}

            SERIALIZABLE_WEAK_FIELD(signed char, value);
        };
        bool operator==(const SignedCharWeakClass &left, const SignedCharWeakClass &right)
        {
            return left.value == right.value;
        }
        SERIALIZABLE_CLASS(IntWeakClass)
        {
        public:
            IntWeakClass(int value)
                :value(value)
            {}

            SERIALIZABLE_WEAK_FIELD(int, value);
        };
        bool operator==(const IntWeakClass &left, const IntWeakClass &right)
        {
            return left.value == right.value;
        }

        SERIALIZABLE_CLASS(SignedCharStrongClass)
        {
        public:
            SignedCharStrongClass(signed char value)
                :value(value)
            {}

            SERIALIZABLE_FIELD(signed char, value);
        };
        SERIALIZABLE_CLASS(IntStrongClass)
        {
        public:
            IntStrongClass(int value)
                :value(value)
            {}

            SERIALIZABLE_FIELD(int, value);
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
                serialization::exception::SerializationException);
        CPPUNIT_TEST_EXCEPTION(testWeakFieldsOverflow,
                serialization::exception::SerializationException);
        CPPUNIT_TEST(testClassAllFields);
        CPPUNIT_TEST(testClassMissingFields);
        CPPUNIT_TEST(testWeakClassMissingFields);
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
                std::string(Point::getClassName()));
            CPPUNIT_ASSERT_EQUAL(std::string("Line"),
                std::string(Line::getClassName()));
            CPPUNIT_ASSERT_EQUAL(std::string("Vector"),
                std::string(Vector::getClassName()));
        }

        void testWeakFieldsShortToInt()
        {
            SignedCharWeakClass charValue{42};
            IntWeakClass intValue{0};
            serialization::toDeserializer(charValue)>>intValue;
            CPPUNIT_ASSERT(IntWeakClass{42} == intValue);
        }

        void testWeakFieldsIntToShort()
        {
            IntWeakClass intValue{42};
            SignedCharWeakClass charValue{0};
            serialization::toDeserializer(intValue)>>charValue;
            CPPUNIT_ASSERT(SignedCharWeakClass{42} == charValue);
        }

        void testStrongFields()
        {
            SignedCharStrongClass charValue{42};
            IntStrongClass intValue{0};
            serialization::DeserializationTrait<SignedCharStrongClass>::toDeserializer(charValue)>>intValue;
        }

        void testWeakFieldsOverflow()
        {
            if constexpr(std::numeric_limits<signed char>::max() < std::numeric_limits<int>::max())
            {
                IntWeakClass intValue{std::numeric_limits<int>::max()};
                SignedCharWeakClass charValue{0};
                serialization::DeserializationTrait<IntWeakClass>::toDeserializer(intValue)>>charValue;
            }
            else
            {
                // not possible to test on this platform
                throw exception::SerializerException(Context());
            }
        }

        void testClassAllFields()
        {
            FieldCl exp(42, 43, 84, 101);
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
            FieldWeakCl exp(42, 43, 131, 1001);
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

    private:
        SERIALIZABLE_CLASS(FieldCl)
        {
        public:
            FieldCl() = default;
            FieldCl(int a, int b, int c, int d)
                :a(a), b(b), c(c), d(d)
            {}
            SERIALIZABLE_FIELD(int, a);
            SERIALIZABLE_FIELD(int, b);
            SERIALIZABLE_FIELD(int, c);
            SERIALIZABLE_FIELD(int, d);

            bool operator==(const FieldCl &that) const
            {
                return a == that.a && b == that.b && c == that.c && d == that.d;
            }
        };

        SERIALIZABLE_WEAK_CLASS(FieldWeakCl)
        {
        public:
            FieldWeakCl() = default;
            FieldWeakCl(int a, int b, int c, int d)
                :a(a), b(b), c(c), d(d)
            {}
            SERIALIZABLE_FIELD(int, a);
            SERIALIZABLE_FIELD(int, b);
            SERIALIZABLE_FIELD(int, c);
            SERIALIZABLE_FIELD(int, d);

            bool operator==(const FieldWeakCl &that) const
            {
                return a == that.a && b == that.b && c == that.c && d == that.d;
            }
        };

        template<typename T>
        bool validateSerialization(const T &value)
        {
            T actual;
            auto serializer = serialization::toSerializer(actual);
            serializer<<value;
            return value == actual;
        }

        template<typename T>
        bool validateDeserialization(const T &value)
        {
            T v(value);
            const T expected(v);
            T actual;
            serialization::toDeserializer(v)>>actual;
            return expected == actual;
        }
    };
    CPPUNIT_TEST_SUITE_REGISTRATION(MetaObjectSerializerTest);
}
