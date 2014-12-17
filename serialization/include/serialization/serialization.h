#ifndef SERIALIZATION_SERIALIZATION_H
#define SERIALIZATION_SERIALIZATION_H

#include <memory>
#include <cassert>
#include <string>
#include <stdint.h>
#include <vector>

#include "serialization/ISerializer.h"
#include "serialization/exception/SerializationException.h"

namespace serialization
{
    class MetaObject;
    class Context;

    // use two types, one for relaxed size types like bool, char, etc
    // and one for strict types like int8_t, int16_t, etc.
    void write(ISerializer &serializer, bool value,
        const Context &context);
    void write(ISerializer &serializer, char value,
        const Context &context);
    void write(ISerializer &serializer, signed char value,
        const Context &context);
    void write(ISerializer &serializer, unsigned char value,
        const Context &context);
    void write(ISerializer &serializer, short value,
        const Context &context);
    void write(ISerializer &serializer, unsigned short value,
        const Context &context);
    void write(ISerializer &serializer, int value,
        const Context &context);
    void write(ISerializer &serializer, unsigned int value,
        const Context &context);
    void write(ISerializer &serializer, long value,
        const Context &context);
    void write(ISerializer &serializer, unsigned long value,
        const Context &context);
    void write(ISerializer &serializer, long long value,
        const Context &context);
    void write(ISerializer &serializer, unsigned long long value,
        const Context &context);
    void write(ISerializer &serializer, float value,
        const Context &context);
    void write(ISerializer &serializer, double value,
        const Context &context);
    void write(ISerializer &serializer, long double value,
        const Context &context);
    void write(ISerializer &serializer, const std::string &value,
        const Context &context);
    template<typename T>
    void write(ISerializer &serializer,
        const T &value, const Context &context);

    // TODO: can use type traits to find "value" types
    std::unique_ptr<ISerializer> beginCollection(bool value);
    std::unique_ptr<ISerializer> beginCollection(char value);
    std::unique_ptr<ISerializer> beginCollection(signed char value);
    std::unique_ptr<ISerializer> beginCollection(unsigned char value);
    std::unique_ptr<ISerializer> beginCollection(short value);
    std::unique_ptr<ISerializer> beginCollection(unsigned short value);
    std::unique_ptr<ISerializer> beginCollection(int value);
    std::unique_ptr<ISerializer> beginCollection(unsigned int value);
    std::unique_ptr<ISerializer> beginCollection(long value);
    std::unique_ptr<ISerializer> beginCollection(unsigned long value);
    std::unique_ptr<ISerializer> beginCollection(long long value);
    std::unique_ptr<ISerializer> beginCollection(unsigned long long value);
    std::unique_ptr<ISerializer> beginCollection(float value);
    std::unique_ptr<ISerializer> beginCollection(double value);
    std::unique_ptr<ISerializer> beginCollection(long double value);
    std::unique_ptr<ISerializer> beginCollection(std::string &value);
    template<typename T>
    std::unique_ptr<ISerializer> beginCollection(T &value);
}

void operator<<(serialization::ISerializer &serializer,
    const serialization::MetaObject &object);
void operator>>(const serialization::ISerializer &deserializer,
    serialization::MetaObject &object);

#include "serialization/factory.h"

namespace serialization
{
    template<typename T>
    void write(ISerializer &serializer,
        const T &value, const Context &context)
    {
        std::unique_ptr<const ISerializer> s(factory::createSerializer(value));
        if(!s.get())
            throw exception::SerializationException(context);
        s->visit(serializer, context);
    }

    template<typename T>
    std::unique_ptr<ISerializer> beginCollection(T &value)
    {
        return factory::createSerializer(value);
    }
}

#endif
