#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include "serialization/MetaObject.h"
#include "serialization/MetaObjectSerializer.h"
#include "serialization/serialization.h"

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
                SERIALIZABLE_FIELD(std::vector<int>, v);
            };

            SERIALIZABLE_CLASS(PointVector)
            {
            public:
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
            CPPUNIT_TEST_SUITE_END();
        public:
            void testSerializePlain()
            {
                const Point point(42, 43);
                const Point expected(point);
                Point actual;
                MetaObjectSerializer serializer(actual);
                serializer<<point;
                CPPUNIT_ASSERT(expected == actual);
            }

            void testDeserializePlain()
            {
                Point point(42, 43);
                const Point expected(point);
                Point actual;
                MetaObjectSerializer serializer(point);
                serializer>>actual;
                CPPUNIT_ASSERT(expected == actual);
            }

            void testSerializeComplex()
            {
                const Line line(Point(1, 2), Point(3, 4));
                const Line expected(line);
                Line actual;
                MetaObjectSerializer serializer(actual);
                serializer<<line;
                CPPUNIT_ASSERT(expected == actual);
            }

            void testDeserializeComplex()
            {
                Line line(Point(1, 2), Point(3, 4));
                const Line expected(line);
                Line actual;
                MetaObjectSerializer serializer(line);
                serializer>>actual;
                CPPUNIT_ASSERT(expected == actual);
            }

            void testSerializeCollectionPlain()
            {
                const Vector v;
                const Vector expected(v);
                Vector actual;
                MetaObjectSerializer serializer(actual);
                serializer<<v;
                CPPUNIT_ASSERT(expected == actual);
            }

            void testDeserializeCollectionPlain()
            {
                Vector v;
                const Vector expected(v);
                Vector actual;
                MetaObjectSerializer serializer(v);
                serializer>>actual;
                CPPUNIT_ASSERT(expected == actual);
            }

            void testSerializeCollectionComplex()
            {
                const PointVector v;
                const PointVector expected(v);
                PointVector actual;
                MetaObjectSerializer serializer(actual);
                serializer<<v;
                CPPUNIT_ASSERT(expected == actual);
            }

            void testDeserializeCollectionComplex()
            {
                PointVector v;
                const PointVector expected(v);
                PointVector actual;
                MetaObjectSerializer serializer(v);
                serializer>>actual;
                CPPUNIT_ASSERT(expected == actual);
            }
        };
        CPPUNIT_TEST_SUITE_REGISTRATION(MetaObjectSerializerTest);
    }
}
