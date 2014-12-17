#ifndef SERIALIZATION_ISERIALIZER_H
#define SERIALIZATION_ISERIALIZER_H

#include <memory>
#include <string>

#include "serialization/Context.h"

namespace serialization
{
    class ISerializer
    {
    public:
        virtual ~ISerializer(){}

        virtual std::auto_ptr<ISerializer> beginCollection(Context::Type type,
            const Context &context) = 0;
        
        virtual void visit(ISerializer &serializer,
            const serialization::Context &context) const = 0;

        virtual Context::Type contextType() const = 0;

        // TODO: add value container type, prevent interface grow
//      virtual void writeNull(const Context &context) = 0;

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
        // TODO: really had to separate the string type
        virtual void write(const std::string &value, const Context &context) = 0;
    };
}

#endif
