#ifndef SERIALIZATION_BASESERIALIZER_HPP
#define SERIALIZATION_BASESERIALIZER_HPP

#include "serialization/context.hpp"
#include "serialization/serializer.hpp"
#include "serialization/deserializer.hpp"
#include "serialization/exception/serializer_exception.hpp"

namespace serialization
{
    class BaseSerializer: public ISerializer
    {
    public:
        virtual ~BaseSerializer() = 0;

        virtual void prepareContext(Context::Type)
        {}

        void write(const IDeserializer &value, const Context &context) override
        {
            if(context.getType() == Context::TYPE_NONE)
            {
                prepareContext(value.contextType());
                value.visit(*this);
            }
            else
            {
                throw exception::SerializerException(context, "invalid context type");
            }
        }
        void write(Null, const Context &context) override
        {
            throw prepareInvalidValueException(context);
        }
        void write(bool, const Context &context) override
        {
            throw prepareInvalidValueException(context);
        }
        void write(char, const Context &context) override
        {
            throw prepareInvalidValueException(context);
        }
        void write(signed char, const Context &context) override
        {
            throw prepareInvalidValueException(context);
        }
        void write(unsigned char, const Context &context) override
        {
            throw prepareInvalidValueException(context);
        }
        void write(short, const Context &context) override
        {
            throw prepareInvalidValueException(context);
        }
        void write(unsigned short, const Context &context) override
        {
            throw prepareInvalidValueException(context);
        }
        void write(int, const Context &context) override
        {
            throw prepareInvalidValueException(context);
        }
        void write(unsigned int, const Context &context) override
        {
            throw prepareInvalidValueException(context);
        }
        void write(long, const Context &context) override
        {
            throw prepareInvalidValueException(context);
        }
        void write(unsigned long, const Context &context) override
        {
            throw prepareInvalidValueException(context);
        }
        void write(long long, const Context &context) override
        {
            throw prepareInvalidValueException(context);
        }
        void write(unsigned long long, const Context &context) override
        {
            throw prepareInvalidValueException(context);
        }
        void write(float, const Context &context) override
        {
            throw prepareInvalidValueException(context);
        }
        void write(double, const Context &context) override
        {
            throw prepareInvalidValueException(context);
        }
        void write(long double, const Context &context) override
        {
            throw prepareInvalidValueException(context);
        }
        void write(const std::string&, const Context &context) override
        {
            throw prepareInvalidValueException(context);
        }

    private:
        static exception::SerializerException prepareInvalidValueException(
                const Context &context)
        {
            return exception::SerializerException(context, "invalid value");
        }
    };
}

#endif