#ifndef XSERIALIZATION_BASESERIALIZER_HPP
#define XSERIALIZATION_BASESERIALIZER_HPP

#include "xserialization/context.hpp"
#include "xserialization/serializer.hpp"
#include "xserialization/deserializer.hpp"
#include "xserialization/exception/serializer_exception.hpp"

namespace xserialization
{
    class BaseSerializer: public ISerializer
    {
    public:
        ~BaseSerializer() override = 0;

        void write(const IDeserializer &value, const Context &context) override
        {
            if(context.getType() == Context::TYPE_NONE)
            {
                if(!prepareContext(value.contextType()))
                {
                    throw prepareInvalidValueException(context);
                }
                value.visit(*this);
            }
            else
            {
                throw prepareInvalidValueException(context);
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

    protected:
        virtual bool prepareContext(Context::Type)
        {
            return true;
        }

    private:
        static exception::SerializerException prepareInvalidValueException(
                const Context &context)
        {
            return {context, "invalid value"};
        }
    };
}

#endif
