#ifndef SERIALIZATION_INNER_METAOBJECTSERIALIZER_HPP
#define SERIALIZATION_INNER_METAOBJECTSERIALIZER_HPP

#include <iterator>
#include <string>
#include <unordered_set>
#include <type_traits>

#include "serialization/serializer.hpp"
#include "serialization/context.hpp"
#include "serialization/exception/serializer_exception.hpp"
#include "serialization/inner/meta_object.hpp"

namespace serialization::inner
{
    class MetaObjectSerializer: public ISerializer
    {
    public:
        explicit MetaObjectSerializer(MetaObject &object)
            :object(object), unusedFields(prepareUnusedFields(object))
        {}

        Context::Type contextType() const override;

        void write(const IDeserializer &value, const Context &context) override
        { return writeValue(context, value); }
        void write(Null, const Context &context) override
        { throw exception::SerializerException(context, "invalid null write"); }
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
        template<typename V>
        void writeValue(const Context &context, const V &value);

        bool allFieldsUsed() const;
        bool isOpenObject() const;

    private:
        static std::unordered_set<std::string> prepareUnusedFields(
                const MetaObject &object);

    private:
        MetaObject &object;
        std::unordered_set<std::string> unusedFields;
    };

    template<typename V>
    void MetaObjectSerializer::writeValue(const Context &context, const V &value)
    {
        if(context.getType() == Context::TYPE_NONE)
        {
            if constexpr(std::is_same_v<std::decay_t<V>, IDeserializer>)
            {
                if(value.contextType() == Context::TYPE_NAME)
                {
                    MetaObjectSerializer s(object);
                    value.visit(s);
                    if(!s.allFieldsUsed())
                    {
                        throw exception::SerializerException(context,
                                "missing fields");
                    }
                }
                else
                {
                    throw exception::SerializerException(context,
                            "invalid value");
                }
            }
            else
            {
                throw exception::SerializerException(context, "invalid value");
            }
            return;
        }
        if(context.getType() != Context::TYPE_NAME)
        {
            throw exception::SerializerException(context, "invalid context type");
        }
        const auto &fields = object.fields();
        auto iter = fields.find(context.getName());
        if(iter != std::end(fields))
        {
            if(!iter->second->write(object, value))
            {
                throw exception::SerializerException(context, "invalid field write");
            }
            unusedFields.erase(context.getName());
        }
        else
        {
            if(!isOpenObject())
            {
                throw exception::SerializerException(context, "unknown field");
            }
        }
    }
}

#endif
