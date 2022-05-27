#include <cassert>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

#include "serialization/MetaObject.h"
#include "serialization/serialization.h"
#include "serialization/ISerializer.h"
#include "serialization/exception/SerializationException.h"
#include "variant/Variant.h"

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

    class StringSerializer: public serialization::ISerializer
    {
    public:
        StringSerializer(std::ostream &ostream)
            :first(true), type(serialization::Context::TYPE_NONE),
            ostream(ostream)
        {
        }

        StringSerializer(std::ostream &ostream, serialization::Context::Type type,
            const serialization::Context &context)
            :first(true), type(type), ostream(ostream)
        {
            if(context.getType() == serialization::Context::TYPE_NAME)
                ostream<<context.getName()<<": ";
            if(type == serialization::Context::TYPE_NAME)
                ostream<<'{';
            else if(type == serialization::Context::TYPE_INDEX)
                ostream<<'[';
        }

        virtual ~StringSerializer()
        {
            if(type == serialization::Context::TYPE_NAME)
                ostream<<'}';
            else if(type == serialization::Context::TYPE_INDEX)
                ostream<<']';
        }

        virtual std::unique_ptr<serialization::ISerializer> beginCollection(
            serialization::Context::Type type,
            const serialization::Context &context)
        {
            writeSeparator();
            return std::auto_ptr<serialization::ISerializer>(
                new StringSerializer(ostream, type, context));
        }

        virtual serialization::Context::Type contextType() const
        {
            return type;
        }

        virtual void write(bool value, const serialization::Context &context)
        {
            writeValue(value, context);
        }
        virtual void write(char value, const serialization::Context &context)
        {
            writeValue(value, context);
        }
        virtual void write(signed char value, const serialization::Context &context)
        {
            writeValue(value, context);
        }
        virtual void write(unsigned char value, const serialization::Context &context)
        {
            writeValue(value, context);
        }
        virtual void write(short value, const serialization::Context &context)
        {
            writeValue(value, context);
        }
        virtual void write(unsigned short value, const serialization::Context &context)
        {
            writeValue(value, context);
        }
        virtual void write(int value, const serialization::Context &context)
        {
            writeValue(value, context);
        }
        virtual void write(unsigned int value, const serialization::Context &context)
        {
            writeValue(value, context);
        }
        virtual void write(long value, const serialization::Context &context)
        {
            writeValue(value, context);
        }
        virtual void write(unsigned long value, const serialization::Context &context)
        {
            writeValue(value, context);
        }
        virtual void write(long long value, const serialization::Context &context)
        {
            writeValue(value, context);
        }
        virtual void write(unsigned long long value, const serialization::Context &context)
        {
            writeValue(value, context);
        }
        virtual void write(float value, const serialization::Context &context)
        {
            writeValue(value, context);
        }
        virtual void write(double value, const serialization::Context &context)
        {
            writeValue(value, context);
        }
        virtual void write(long double value, const serialization::Context &context)
        {
            writeValue(value, context);
        }
        virtual void write(const std::string &value,
            const serialization::Context &context)
        {
            writeValue(value, context);
        }

        virtual void visit(ISerializer&,
            const serialization::Context&) const
        {
            throw serialization::exception::SerializationException(serialization::Context(""));
        }

    private:
        void writeSeparator()
        {
            if(!first)
                ostream<<", ";
            else
                first = false;
        }

        template<typename T>
        void writeValue(const T &value, const serialization::Context &context)
        {
            writeSeparator();
            if(context.getType() == serialization::Context::TYPE_NAME)
                ostream<<context.getName()<<':'<<value;
            else if(context.getType() == serialization::Context::TYPE_INDEX)
                ostream<<value;
            else
                throw serialization::exception::SerializationException(
                    context);
        }

    private:
        bool first;
        serialization::Context::Type type;
        std::ostream &ostream;
    };

    template<typename T>
    void print(std::ostream &ostream, const T &value)
    {
        ostream<<value.getClassName()<<": ";
        StringSerializer serializer(ostream);
        serializer<<value;
    }

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

int main()
{
    std::stringstream ostream;
    Point p(42, 43);
    print(ostream, p);
    ostream<<std::endl;
    Line l(Point(1, 2), Point(3, 4));
    print(ostream, l);
    ostream<<std::endl;
    Vector v(generate<Vector::ValueType>(42, 10));
    print(ostream, v);
    ostream<<std::endl;
    PointVector pv(generate<PointVector::ValueType>(Point(), 10));
    print(ostream, pv);
    std::cout<<ostream.str()<<std::endl;
    return 0;
}
