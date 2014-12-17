#ifndef SERIALIZATION_METAOBJECTSERIALIZER_H
#define SERIALIZATION_METAOBJECTSERIALIZER_H

#include "serialization/ISerializer.h"

namespace serialization
{
    class MetaObject;

    class MetaObjectSerializer: public ISerializer
    {
    public:
        MetaObjectSerializer(MetaObject &object);

        virtual std::unique_ptr<ISerializer> beginCollection(
            Context::Type type, const Context &context);

        virtual void visit(ISerializer &serializer,
            const serialization::Context &context) const;

        virtual Context::Type contextType() const;

        virtual void writeNull(const Context &context);

        virtual void write(bool value, const Context &context);
        virtual void write(char value, const Context &context);
        virtual void write(signed char value, const Context &context);
        virtual void write(unsigned char value, const Context &context);
        virtual void write(short value, const Context &context);
        virtual void write(unsigned short value, const Context &context);
        virtual void write(int value, const Context &context);
        virtual void write(unsigned int value, const Context &context);
        virtual void write(long value, const Context &context);
        virtual void write(unsigned long value, const Context &context);
        virtual void write(long long value, const Context &context);
        virtual void write(unsigned long long value, const Context &context);
        virtual void write(float value, const Context &context);
        virtual void write(double value, const Context &context);
        virtual void write(long double value, const Context &context);
        virtual void write(const std::string &value,
            const Context &context);

    private:
        MetaObject &object;
    };
}

#endif
