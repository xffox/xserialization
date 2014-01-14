#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

#include "serialization/MetaObject.h"
#include "serialization/serialization.h"
#include "serialization/ISerializer.h"
#include "serialization/exception/SerializationException.h"

namespace
{
    SERIALIZABLE_CLASS(Point)
    {
    public:
        Point()
        :x(0), y(0), s(), b(false)
        {}
        Point(int x, int y, const std::string &s, bool b)
        :x(x), y(y), s(s), b(b)
        {}

        SERIALIZABLE_FIELD(int, x);
        SERIALIZABLE_FIELD(int, y);
        SERIALIZABLE_FIELD(std::string, s);
        SERIALIZABLE_FIELD(bool, b);
        SERIALIZABLE_FIELD(std::vector<int>, v);
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

    class StringSerializer: public serialization::ISerializer
    {
    public:
        StringSerializer(std::ostream &ostream)
            :first(true), type(serialization::Context::TYPE_NAME),
            ostream(ostream)
        {
            ostream<<"{";
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

        virtual std::auto_ptr<serialization::ISerializer> beginNamedCollection(const serialization::Context &context)
        {
            writeSeparator();
            return std::auto_ptr<serialization::ISerializer>(new StringSerializer(ostream,
                    serialization::Context::TYPE_NAME, context));
        }
        virtual std::auto_ptr<serialization::ISerializer> beginIndexedCollection(const serialization::Context &context)
        {
            writeSeparator();
            return std::auto_ptr<serialization::ISerializer>(new StringSerializer(ostream,
                    serialization::Context::TYPE_INDEX, context));
        }

        virtual serialization::Context::Type contextType() const
        {
            return type;
        }

        virtual void writeNull(const serialization::Context&)
        {
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
        StringSerializer serializer(ostream);
        serializer<<value;
    }
}

int main()
{
    std::stringstream ostream;
    Point p(42, 43, "foobar", true);
    p.v.push_back(12);
    p.v.push_back(13);
    print(ostream, p);
    ostream<<std::endl;
    Line l(Point(1, 2, "a", false), Point(3, 4, "b", true));
    print(ostream, l);
    std::cout<<ostream.str()<<std::endl;
    return 0;
}
