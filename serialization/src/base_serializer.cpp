#include "xserialization/base_serializer.hpp"

#include "xserialization/inner/atom_deserializer.hpp"
#include "xserialization/exception/serializer_exception.hpp"

namespace xserialization
{
    BaseSerializer::~BaseSerializer() = default;

    void BaseSerializer::write(const IDeserializer &value,
        const Context &context)
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
    void BaseSerializer::write(Null value, const Context &context)
    {
        writeAtom(value, context);
    }
    void BaseSerializer::write(bool value, const Context &context)
    {
        writeAtom(value, context);
    }
    void BaseSerializer::write(char value, const Context &context)
    {
        writeAtom(value, context);
    }
    void BaseSerializer::write(signed char value, const Context &context)
    {
        writeAtom(value, context);
    }
    void BaseSerializer::write(unsigned char value, const Context &context)
    {
        writeAtom(value, context);
    }
    void BaseSerializer::write(short value, const Context &context)
    {
        writeAtom(value, context);
    }
    void BaseSerializer::write(unsigned short value, const Context &context)
    {
        writeAtom(value, context);
    }
    void BaseSerializer::write(int value, const Context &context)
    {
        writeAtom(value, context);
    }
    void BaseSerializer::write(unsigned int value, const Context &context)
    {
        writeAtom(value, context);
    }
    void BaseSerializer::write(long value, const Context &context)
    {
        writeAtom(value, context);
    }
    void BaseSerializer::write(unsigned long value, const Context &context)
    {
        writeAtom(value, context);
    }
    void BaseSerializer::write(long long value, const Context &context)
    {
        writeAtom(value, context);
    }
    void BaseSerializer::write(unsigned long long value, const Context &context)
    {
        writeAtom(value, context);
    }
    void BaseSerializer::write(float value, const Context &context)
    {
        writeAtom(value, context);
    }
    void BaseSerializer::write(double value, const Context &context)
    {
        writeAtom(value, context);
    }
    void BaseSerializer::write(long double value, const Context &context)
    {
        writeAtom(value, context);
    }
    void BaseSerializer::write(const std::string& value, const Context &context)
    {
        writeAtom(value, context);
    }

    bool BaseSerializer::prepareContext(Context::Type)
    {
        return true;
    }

    exception::TypeSerializerException BaseSerializer::prepareInvalidValueException(
            const Context &context)
    {
        return {context, "invalid value"};
    }

    template<typename T>
    void BaseSerializer::writeAtom(const T &value, const Context &context)
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
}
