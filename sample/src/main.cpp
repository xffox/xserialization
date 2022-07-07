#include <cassert>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <type_traits>

#include "serialization/serialization.hpp"

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
        {}

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

        ~StringSerializer()
        {
            if(type == serialization::Context::TYPE_NAME)
                ostream<<'}';
            else if(type == serialization::Context::TYPE_INDEX)
                ostream<<']';
        }

        serialization::Context::Type contextType() const override
        {
            return type;
        }

        void write(const serialization::IDeserializer &value,
                const serialization::Context &context) override
        {
            writeSeparator();
            StringSerializer serializer(ostream, value.contextType(), context);
            value.visit(serializer);
        }

        void write(serialization::Null, const serialization::Context &context) override
        {
            write("<null>", context);
        }
        void write(bool value, const serialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(char value, const serialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(signed char value, const serialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(unsigned char value, const serialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(short value, const serialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(unsigned short value, const serialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(int value, const serialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(unsigned int value, const serialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(long value, const serialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(unsigned long value, const serialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(long long value, const serialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(unsigned long long value, const serialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(float value, const serialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(double value, const serialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(long double value, const serialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(const std::string &value,
            const serialization::Context &context) override
        {
            writeValue(value, context);
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
                throw serialization::exception::SerializerException(
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
        T res{};
        typename T::value_type current(first);
        for(std::size_t i = 0; i < count; ++i)
            res.push_back(current++);
        return res;
    }

    struct Data
    {
        std::string str;
        int num;
    };

    SERIALIZABLE_CLASS(CustomClass)
    {
    public:
        CustomClass(Data data)
            :data(data)
        {}

        SERIALIZABLE_FIELD(Data, data);
    };

    class DataDeserializer: public serialization::IDeserializer
    {
    public:
        explicit DataDeserializer(const Data &data)
            :data(data)
        {}

        serialization::Context::Type contextType() const override
        {
            return serialization::Context::TYPE_NAME;
        }

        void visit(serialization::ISerializer &serializer) const override
        {
            serializer.write(data.str, "str");
            serializer.write(data.num, "num");
        }
    private:
        const Data &data;
    };

    class DataSerializer: public serialization::BaseSerializer
    {
    public:
        explicit DataSerializer(Data &data)
            :data(data)
        {}

        serialization::Context::Type contextType() const override
        {
            return serialization::Context::TYPE_NAME;
        }

        void write(const serialization::IDeserializer &value,
                const serialization::Context &context) override
        {
            if(context.getType() != serialization::Context::TYPE_NONE)
            {
                throw serialization::exception::SerializerException(context);
            }
            value.visit(*this);
        }
        void write(const std::string &value, const serialization::Context &context) override
        {
            if(context != serialization::Context("str"))
            {
                throw serialization::exception::SerializerException(context);
            }
            data.str = value;
        }
        void write(int value, const serialization::Context &context) override
        {
            if(context != serialization::Context("num"))
            {
                throw serialization::exception::SerializerException(context);
            }
            data.num = value;
        }

    private:
        Data &data;
    };
}

template<>
struct serialization::DeserializationTrait<Data>
{
    using Deserializer = DataDeserializer;

    static Deserializer toDeserializer(const Data &data)
    {
        return DataDeserializer(data);
    }
};

template<>
struct serialization::SerializationTrait<Data>
{
    using Serializer = DataSerializer;

    static Serializer toSerializer(Data &data)
    {
        return DataSerializer(data);
    }
};

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
    ostream<<std::endl;
    print(ostream, CustomClass{Data{"foobar", 42}});
    std::cout<<ostream.str()<<std::endl;
    return 0;
}
