#include <cassert>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <type_traits>
#include <utility>

#include <xserialization/serialization.hpp>

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

        MT_FIELD(x, int, MT_FIELD_ATTR_WEAK);
        MT_FIELD(y, int, MT_FIELD_ATTR_WEAK);
    };

    MT_CLASS(Line)
    {
    public:
        Line()
            :begin(), end()
        {}
        Line(Point begin, Point end)
            :begin(std::move(begin)), end(std::move(end))
        {}

        MT_FIELD(begin, Point);
        MT_FIELD(end, Point);
    };

    MT_CLASS(Vector)
    {
    public:
        using ValueType = std::vector<int>;

    public:
        Vector()
        :v()
        {}
        Vector(std::vector<int> v)
        :v(std::move(v))
        {}

        MT_FIELD(v, std::vector<int>);
    };

    MT_CLASS(PointVector)
    {
    public:
        using ValueType = std::vector<Point>;

    public:
        PointVector()
        :v()
        {}
        PointVector(std::vector<Point> v)
        :v(std::move(v))
        {}

        MT_FIELD(v, std::vector<Point>);
    };

    class StringSerializer: public xserialization::ISerializer
    {
    public:
        StringSerializer(std::ostream &ostream)
            :first(true), type(xserialization::Context::TYPE_NONE),
            ostream(ostream)
        {}

        StringSerializer(std::ostream &ostream, xserialization::Context::Type type,
            const xserialization::Context &context)
            :first(true), type(type), ostream(ostream)
        {
            if(context.getType() == xserialization::Context::TYPE_NAME)
            {
                ostream<<context.getName()<<": ";
            }
            if(type == xserialization::Context::TYPE_NAME)
            {
                ostream<<'{';
            }
            else if(type == xserialization::Context::TYPE_INDEX)
            {
                ostream<<'[';
            }
        }

        ~StringSerializer() override
        {
            if(type == xserialization::Context::TYPE_NAME)
            {
                ostream<<'}';
            }
            else if(type == xserialization::Context::TYPE_INDEX)
            {
                ostream<<']';
            }
        }

        [[nodiscard]]
        xserialization::Context::Type contextType() const override
        {
            return type;
        }

        void write(const xserialization::IDeserializer &value,
                const xserialization::Context &context) override
        {
            writeSeparator();
            StringSerializer serializer(ostream, value.contextType(), context);
            value.visit(serializer);
        }

        void write(xserialization::Null, const xserialization::Context &context) override
        {
            write(std::string("<null>"), context);
        }
        void write(bool value, const xserialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(char value, const xserialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(signed char value, const xserialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(unsigned char value, const xserialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(short value, const xserialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(unsigned short value, const xserialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(int value, const xserialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(unsigned int value, const xserialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(long value, const xserialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(unsigned long value, const xserialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(long long value, const xserialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(unsigned long long value, const xserialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(float value, const xserialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(double value, const xserialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(long double value, const xserialization::Context &context) override
        {
            writeValue(value, context);
        }
        void write(const std::string &value,
            const xserialization::Context &context) override
        {
            writeValue(value, context);
        }

    private:
        void writeSeparator()
        {
            if(!first)
            {
                ostream<<", ";
            }
            else
            {
                first = false;
            }
        }

        template<typename T>
        void writeValue(const T &value, const xserialization::Context &context)
        {
            writeSeparator();
            if(context.getType() == xserialization::Context::TYPE_NAME)
            {
                ostream<<context.getName()<<':'<<value;
            }
            else if(context.getType() == xserialization::Context::TYPE_INDEX)
            {
                ostream<<value;
            }
            else
            {
                throw xserialization::exception::SerializerException(
                        context);
            }
        }

    private:
        bool first;
        xserialization::Context::Type type;
        std::ostream &ostream;
    };

    template<typename T>
    void print(std::ostream &ostream, const T &value)
    {
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
        {
            res.push_back(current++);
        }
        return res;
    }

    struct Data
    {
        std::string str;
        int num;
    };

    MT_CLASS(CustomClass)
    {
    public:
        explicit CustomClass(Data data)
            :data(std::move(data))
        {}

        MT_FIELD(data, Data);
    };

    class DataDeserializer: public xserialization::IDeserializer
    {
    public:
        explicit DataDeserializer(const Data &data)
            :data(data)
        {}

        [[nodiscard]]
        xserialization::Context::Type contextType() const override
        {
            return xserialization::Context::TYPE_NAME;
        }

        void visit(xserialization::ISerializer &serializer) const override
        {
            serializer.write(data.str, "str");
            serializer.write(data.num, "num");
        }
    private:
        const Data &data;
    };

    class DataSerializer: public xserialization::BaseSerializer
    {
    public:
        explicit DataSerializer(Data &data)
            :data(data)
        {}

        [[nodiscard]]
        xserialization::Context::Type contextType() const override
        {
            return xserialization::Context::TYPE_NAME;
        }

        void write(const xserialization::IDeserializer &value,
                const xserialization::Context &context) override
        {
            if(context.getType() != xserialization::Context::TYPE_NONE)
            {
                throw xserialization::exception::SerializerException(context);
            }
            value.visit(*this);
        }
        void write(const std::string &value, const xserialization::Context &context) override
        {
            if(context != xserialization::Context("str"))
            {
                throw xserialization::exception::SerializerException(context);
            }
            data.str = value;
        }
        void write(int value, const xserialization::Context &context) override
        {
            if(context != xserialization::Context("num"))
            {
                throw xserialization::exception::SerializerException(context);
            }
            data.num = value;
        }

    private:
        Data &data;
    };
}

template<>
struct xserialization::DeserializationTrait<Data>
{
    using Deserializer = DataDeserializer;

    static Deserializer toDeserializer(const Data &data)
    {
        return DataDeserializer(data);
    }
};

template<>
struct xserialization::SerializationTrait<Data>
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
    const Point p(42, 43);
    print(ostream, p);
    ostream<<std::endl;
    Line l(Point(1, 2), Point(3, 4));
    print(ostream, l);
    ostream<<std::endl;
    const Vector v(generate<Vector::ValueType>(42, 10));
    print(ostream, v);
    ostream<<std::endl;
    const PointVector pv(generate<PointVector::ValueType>(Point(), 10));
    print(ostream, pv);
    ostream<<std::endl;
    print(ostream, CustomClass{Data{"foobar", 42}});
    std::cout<<ostream.str()<<std::endl;
    return 0;
}
