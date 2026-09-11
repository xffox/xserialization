#ifndef XSERIALIZATION_BASESERIALIZER_HPP
#define XSERIALIZATION_BASESERIALIZER_HPP

#include "xserialization/context.hpp"
#include "xserialization/serializer.hpp"
#include "xserialization/deserializer.hpp"
#include "xserialization/inner/atom_deserializer.hpp"
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
        void write(Null value, const Context &context) override
        {
            writeAtom(value, context);
        }
        void write(bool value, const Context &context) override
        {
            writeAtom(value, context);
        }
        void write(char value, const Context &context) override
        {
            writeAtom(value, context);
        }
        void write(signed char value, const Context &context) override
        {
            writeAtom(value, context);
        }
        void write(unsigned char value, const Context &context) override
        {
            writeAtom(value, context);
        }
        void write(short value, const Context &context) override
        {
            writeAtom(value, context);
        }
        void write(unsigned short value, const Context &context) override
        {
            writeAtom(value, context);
        }
        void write(int value, const Context &context) override
        {
            writeAtom(value, context);
        }
        void write(unsigned int value, const Context &context) override
        {
            writeAtom(value, context);
        }
        void write(long value, const Context &context) override
        {
            writeAtom(value, context);
        }
        void write(unsigned long value, const Context &context) override
        {
            writeAtom(value, context);
        }
        void write(long long value, const Context &context) override
        {
            writeAtom(value, context);
        }
        void write(unsigned long long value, const Context &context) override
        {
            writeAtom(value, context);
        }
        void write(float value, const Context &context) override
        {
            writeAtom(value, context);
        }
        void write(double value, const Context &context) override
        {
            writeAtom(value, context);
        }
        void write(long double value, const Context &context) override
        {
            writeAtom(value, context);
        }
        void write(const std::string& value, const Context &context) override
        {
            writeAtom(value, context);
        }

    protected:
        virtual bool prepareContext(Context::Type)
        {
            return true;
        }

    private:
        template<typename T>
        void writeAtom(const T &value, const Context &context)
        {
            // Base serializer tries to coalesce handling for IDeserializer and
            // atom values - they both can be used to represent a value, but
            // downstream seralizers can only implement one.
            if(recurse)
            {
                throw prepareInvalidValueException(context);
            }
            recurse = true;
            try
            {
                write(inner::AtomDeserializer(value), context);
            }
            catch(...)
            {
                recurse = false;
                throw;
            }
            recurse = false;
        }

        static exception::TypeSerializerException prepareInvalidValueException(
                const Context &context)
        {
            return {context, "invalid value"};
        }

        bool recurse = false;
    };
}

#endif
