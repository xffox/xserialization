#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <string>

#include "serialization/MetaObject.h"
#include "serialization/MetaObjectSerializer.h"
#include "serialization/serialization.h"

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

namespace serialization
{
    namespace test
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
                    const Point res(*this);
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

        private:
            template<typename T>
            bool validateSerialization(const T &value)
            {
                const T v(value);
                const T expected(v);
                T actual;
                MetaObjectSerializer serializer(actual);
                serializer<<v;
                return expected == actual;
            }

            template<typename T>
            bool validateDeserialization(const T &value)
            {
                T v(value);
                const T expected(v);
                T actual;
                MetaObjectSerializer serializer(static_cast<T&>(v));
                serializer>>actual;
                return expected == actual;
            }
        };
        CPPUNIT_TEST_SUITE_REGISTRATION(MetaObjectSerializerTest);
    }
}
