#ifndef SERIALIZATION_SERIALIZER_HPP
#define SERIALIZATION_SERIALIZER_HPP

#include <string>

#include "serialization/context.hpp"
#include "serialization/null.hpp"

namespace serialization
{
    class IDeserializer;

    class ISerializer
    {
    public:
        virtual ~ISerializer() = default;

        virtual Context::Type contextType() const = 0;

        // TODO: not sure, maybe sized values are better: std::int32_t, etc.
        virtual void write(const IDeserializer &value, const Context &context) = 0;
        virtual void write(Null, const Context &context) = 0;
        virtual void write(bool value, const Context &context) = 0;
        virtual void write(char value, const Context &context) = 0;
        virtual void write(signed char value, const Context &context) = 0;
        virtual void write(unsigned char value, const Context &context) = 0;
        virtual void write(short value, const Context &context) = 0;
        virtual void write(unsigned short value, const Context &context) = 0;
        virtual void write(int value, const Context &context) = 0;
        virtual void write(unsigned int value, const Context &context) = 0;
        virtual void write(long value, const Context &context) = 0;
        virtual void write(unsigned long value, const Context &context) = 0;
        virtual void write(long long value, const Context &context) = 0;
        virtual void write(unsigned long long value, const Context &context) = 0;
        virtual void write(float value, const Context &context) = 0;
        virtual void write(double value, const Context &context) = 0;
        virtual void write(long double value, const Context &context) = 0;
        virtual void write(const std::string &value, const Context &context) = 0;
    };
}

#include "serialization/deserializer.hpp"

#endif
