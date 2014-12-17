#include "serialization/serialization.h"

#include <cassert>

#include "serialization/MetaObject.h"
#include "serialization/MetaObjectSerializer.h"

namespace serialization
{
    // TODO: add beginCollection functions
    void write(ISerializer &serializer, bool value,
        const Context &context)
    {
        serializer.write(value, context);
    }
    void write(ISerializer &serializer, char value,
        const Context &context)
    {
        serializer.write(value, context);
    }
    void write(ISerializer &serializer, signed char value,
        const Context &context)
    {
        serializer.write(value, context);
    }
    void write(ISerializer &serializer, unsigned char value,
        const Context &context)
    {
        serializer.write(value, context);
    }
    void write(ISerializer &serializer, short value,
        const Context &context)
    {
        serializer.write(value, context);
    }
    void write(ISerializer &serializer, unsigned short value,
        const Context &context)
    {
        serializer.write(value, context);
    }
    void write(ISerializer &serializer, int value,
        const Context &context)
    {
        serializer.write(value, context);
    }
    void write(ISerializer &serializer, unsigned int value,
        const Context &context)
    {
        serializer.write(value, context);
    }
    void write(ISerializer &serializer, long value,
        const Context &context)
    {
        serializer.write(value, context);
    }
    void write(ISerializer &serializer, unsigned long value,
        const Context &context)
    {
        serializer.write(value, context);
    }
    void write(ISerializer &serializer, long long value,
        const Context &context)
    {
        serializer.write(value, context);
    }
    void write(ISerializer &serializer, unsigned long long value,
        const Context &context)
    {
        serializer.write(value, context);
    }
    void write(ISerializer &serializer, float value,
        const Context &context)
    {
        serializer.write(value, context);
    }
    void write(ISerializer &serializer, double value,
        const Context &context)
    {
        serializer.write(value, context);
    }
    void write(ISerializer &serializer, long double value,
        const Context &context)
    {
        serializer.write(value, context);
    }
    void write(ISerializer &serializer, const std::string &value,
        const Context &context)
    {
        serializer.write(value, context);
    }

    std::auto_ptr<ISerializer> beginCollection(bool)
    {return std::auto_ptr<ISerializer>();}
    std::auto_ptr<ISerializer> beginCollection(char)
    {return std::auto_ptr<ISerializer>();}
    std::auto_ptr<ISerializer> beginCollection(signed char)
    {return std::auto_ptr<ISerializer>();}
    std::auto_ptr<ISerializer> beginCollection(unsigned char)
    {return std::auto_ptr<ISerializer>();}
    std::auto_ptr<ISerializer> beginCollection(short)
    {return std::auto_ptr<ISerializer>();}
    std::auto_ptr<ISerializer> beginCollection(unsigned short)
    {return std::auto_ptr<ISerializer>();}
    std::auto_ptr<ISerializer> beginCollection(int)
    {return std::auto_ptr<ISerializer>();}
    std::auto_ptr<ISerializer> beginCollection(unsigned int)
    {return std::auto_ptr<ISerializer>();}
    std::auto_ptr<ISerializer> beginCollection(long)
    {return std::auto_ptr<ISerializer>();}
    std::auto_ptr<ISerializer> beginCollection(unsigned long)
    {return std::auto_ptr<ISerializer>();}
    std::auto_ptr<ISerializer> beginCollection(long long)
    {return std::auto_ptr<ISerializer>();}
    std::auto_ptr<ISerializer> beginCollection(unsigned long long)
    {return std::auto_ptr<ISerializer>();}
    std::auto_ptr<ISerializer> beginCollection(float)
    {return std::auto_ptr<ISerializer>();}
    std::auto_ptr<ISerializer> beginCollection(double)
    {return std::auto_ptr<ISerializer>();}
    std::auto_ptr<ISerializer> beginCollection(long double)
    {return std::auto_ptr<ISerializer>();}
    std::auto_ptr<ISerializer> beginCollection(std::string&)
    {return std::auto_ptr<ISerializer>();}
}

void operator<<(serialization::ISerializer &serializer,
    const serialization::MetaObject &object)
{
    write(serializer, object, serialization::Context());
}

void operator>>(const serialization::ISerializer &deserializer,
    serialization::MetaObject &object)
{
    serialization::MetaObjectSerializer s(object);
    deserializer.visit(s, serialization::Context());
}
