#ifndef SERIALIZATION_INNER_METAOBJECT_HPP
#define SERIALIZATION_INNER_METAOBJECT_HPP

#include <cassert>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <tuple>
#include <memory>

#include "serialization/serializer.hpp"
#include "serialization/deserializer.hpp"
#include "serialization/context.hpp"
#include "serialization/exception/serialization_exception.hpp"

namespace serialization::inner
{
    class MetaObject: public ISerializer, public IDeserializer
    {
    public:
        virtual ~MetaObject() = 0;

        Context::Type contextType() const override;

    protected:
        class IField
        {
        public:
            virtual ~IField() = 0;

            virtual void visit(ISerializer &serializer, const MetaObject &object) = 0;

            virtual void write(MetaObject&, const IDeserializer&)
            {
                // TODO: proper context
                throw exception::SerializationException(Context());
            }
            virtual void write(MetaObject&, bool)
            {
                throw exception::SerializationException(Context());
            }
            virtual void write(MetaObject&, char)
            {
                throw exception::SerializationException(Context());
            }
            virtual void write(MetaObject&, signed char)
            {
                throw exception::SerializationException(Context());
            }
            virtual void write(MetaObject&, unsigned char)
            {
                throw exception::SerializationException(Context());
            }
            virtual void write(MetaObject&, short)
            {
                throw exception::SerializationException(Context());
            }
            virtual void write(MetaObject&, unsigned short)
            {
                throw exception::SerializationException(Context());
            }
            virtual void write(MetaObject&, int)
            {
                throw exception::SerializationException(Context());
            }
            virtual void write(MetaObject&, unsigned int)
            {
                throw exception::SerializationException(Context());
            }
            virtual void write(MetaObject&, long)
            {
                throw exception::SerializationException(Context());
            }
            virtual void write(MetaObject&, unsigned long)
            {
                throw exception::SerializationException(Context());
            }
            virtual void write(MetaObject&, long long)
            {
                throw exception::SerializationException(Context());
            }
            virtual void write(MetaObject&, unsigned long long)
            {
                throw exception::SerializationException(Context());
            }
            virtual void write(MetaObject&, float)
            {
                throw exception::SerializationException(Context());
            }
            virtual void write(MetaObject&, double)
            {
                throw exception::SerializationException(Context());
            }
            virtual void write(MetaObject&, long double)
            {
                throw exception::SerializationException(Context());
            }
            virtual void write(MetaObject&, const std::string&)
            {
                throw exception::SerializationException(Context());
            }
        };
        typedef std::unordered_map<std::string, IField*> FieldMap;
    };

    template<class T>
    class MetaObjectBase: public MetaObject
    {
    public:
        virtual ~MetaObjectBase() = 0;

        void visit(ISerializer &serializer) const override;

        void write(const IDeserializer &value, const Context &context) override
        { return writeValue(context, value); }
        void write(Null, const Context &context) override
        { throw exception::SerializationException(context); }
        void write(bool value, const Context &context) override
        { return writeValue(context, value); }
        void write(char value, const Context &context) override
        { return writeValue(context, value); }
        void write(signed char value, const Context &context) override
        { return writeValue(context, value); }
        void write(unsigned char value, const Context &context) override
        { return writeValue(context, value); }
        void write(short value, const Context &context) override
        { return writeValue(context, value); }
        void write(unsigned short value, const Context &context) override
        { return writeValue(context, value); }
        void write(int value, const Context &context) override
        { return writeValue(context, value); }
        void write(unsigned int value, const Context &context) override
        { return writeValue(context, value); }
        void write(long value, const Context &context) override
        { return writeValue(context, value); }
        void write(unsigned long value, const Context &context) override
        { return writeValue(context, value); }
        void write(long long value, const Context &context) override
        { return writeValue(context, value); }
        void write(unsigned long long value, const Context &context) override
        { return writeValue(context, value); }
        void write(float value, const Context &context) override
        { return writeValue(context, value); }
        void write(double value, const Context &context) override
        { return writeValue(context, value); }
        void write(long double value, const Context &context) override
        { return writeValue(context, value); }
        void write(const std::string &value, const Context &context) override
        { return writeValue(context, value); }

    protected:
        using BaseClass = T;

    protected:
        static FieldMap &getFields()
        {
            static FieldMap fields;
            return fields;
        }

        static void addField(const std::string &name, IField &field)
        {
            // TODO: assert uniquety
            getFields().insert(std::pair<std::string, IField*>(name, &field));
        }

        template<typename V>
        void writeValue(const Context &context, const V &value);
    };

    template<class T>
    MetaObjectBase<T>::~MetaObjectBase() = default;

    template<typename T>
    void MetaObjectBase<T>::visit(ISerializer &serializer) const
    {
        if(serializer.contextType() != Context::TYPE_NAME)
        {
            // TODO: proper exception
            throw exception::SerializationException(Context());
        }
        for(typename FieldMap::const_iterator iter = getFields().begin(),
            endIter = getFields().end(); iter != endIter; ++iter)
        {
            iter->second->visit(serializer, *this);
        }
    }

    template<typename T>
    template<typename V>
    void MetaObjectBase<T>::writeValue(const Context &context, const V &value)
    {
        if(context.getType() == Context::TYPE_NONE)
        {
            if constexpr(std::is_same_v<std::decay_t<V>, IDeserializer>)
            {
                if(value.contextType() == Context::TYPE_NAME)
                {
                    value.visit(*this);
                }
                else
                {
                    throw exception::SerializationException(context);
                }
            }
            else
            {
                throw exception::SerializationException(context);
            }
            return;
        }
        if(context.getType() != Context::TYPE_NAME)
        {
            throw exception::SerializationException(context);
        }
        FieldMap::iterator iter = getFields().find(context.getName());
        if(iter != getFields().end())
        {
            iter->second->write(*this, value);
            return;
        }
        throw exception::SerializationException(context);
    }
}

#endif
