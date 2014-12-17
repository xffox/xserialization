#include "serialization/MetaObjectSerializer.h"

#include <cassert>

#include "serialization/MetaObject.h"
#include "serialization/exception/SerializationException.h"

namespace serialization
{
    namespace
    {
        template<typename T>
        void writeByName(MetaObject &object, const T &value,
            const Context &context)
        {
            bool res = false;
            if(context.getType() == Context::TYPE_NAME)
                res = object.write(context.getName(), value);
            if(!res)
                throw exception::SerializationException(context);
        }
    }

    MetaObjectSerializer::MetaObjectSerializer(MetaObject &object)
        :object(object)
    {}

    std::auto_ptr<ISerializer> MetaObjectSerializer::beginCollection(
        Context::Type type, const Context &context)
    {
        if(context.getType() == Context::TYPE_NAME)
        {
            std::auto_ptr<ISerializer> serializer =
                object.beginCollection(context.getName());
            if(serializer.get() && serializer->contextType() == type)
                return serializer;
        }
        else if(context.getType() == Context::TYPE_NONE)
        {
            return std::auto_ptr<ISerializer>(
                new MetaObjectSerializer(object));
        }
        throw exception::SerializationException(context);
    }

    void MetaObjectSerializer::visit(ISerializer &serializer,
        const serialization::Context &context) const
    {
        std::auto_ptr<serialization::ISerializer> s =
            serializer.beginCollection(Context::TYPE_NAME, context);
        if(s.get())
            object.visit(*s);
        else
            throw exception::SerializationException(context);
    }

    Context::Type MetaObjectSerializer::contextType() const
    {
        return Context::TYPE_NAME;
    }

    void MetaObjectSerializer::writeNull(const Context &context)
    {
        bool ok = false;
        if(context.getType() == Context::TYPE_NAME)
            ok = object.writeNull(context.getName());
        if(!ok)
            throw exception::SerializationException(context);
    }

    void MetaObjectSerializer::write(bool value, const Context &context)
    {
        writeByName(object, value, context);
    }

    void MetaObjectSerializer::write(char value, const Context &context)
    {
        writeByName(object, value, context);
    }

    void MetaObjectSerializer::write(signed char value, const Context &context)
    {
        writeByName(object, value, context);
    }

    void MetaObjectSerializer::write(unsigned char value, const Context &context)
    {
        writeByName(object, value, context);
    }

    void MetaObjectSerializer::write(short value, const Context &context)
    {
        writeByName(object, value, context);
    }

    void MetaObjectSerializer::write(unsigned short value, const Context &context)
    {
        writeByName(object, value, context);
    }

    void MetaObjectSerializer::write(int value, const Context &context)
    {
        writeByName(object, value, context);
    }

    void MetaObjectSerializer::write(unsigned int value, const Context &context)
    {
        writeByName(object, value, context);
    }

    void MetaObjectSerializer::write(long value, const Context &context)
    {
        writeByName(object, value, context);
    }

    void MetaObjectSerializer::write(unsigned long value, const Context &context)
    {
        writeByName(object, value, context);
    }

    void MetaObjectSerializer::write(long long value, const Context &context)
    {
        writeByName(object, value, context);
    }

    void MetaObjectSerializer::write(unsigned long long value, const Context &context)
    {
        writeByName(object, value, context);
    }

    void MetaObjectSerializer::write(float value, const Context &context)
    {
        writeByName(object, value, context);
    }

    void MetaObjectSerializer::write(double value, const Context &context)
    {
        writeByName(object, value, context);
    }

    void MetaObjectSerializer::write(long double value, const Context &context)
    {
        writeByName(object, value, context);
    }

    void MetaObjectSerializer::write(const std::string &value,
        const Context &context)
    {
        writeByName(object, value, context);
    }
}
