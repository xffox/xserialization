#ifndef XSERIALIZATION_INNER_METAOBJECTSERIALIZER_HPP
#define XSERIALIZATION_INNER_METAOBJECTSERIALIZER_HPP

#include <algorithm>
#include <iterator>
#include <string>
#include <unordered_set>
#include <type_traits>

#include "xserialization/serializer.hpp"
#include "xserialization/context.hpp"
#include "xserialization/exception/serializer_exception.hpp"
#include "xserialization/inner/meta_object.hpp"
#include "xserialization/inner/attribute.hpp"

namespace xserialization::inner
{
    template<typename T>
    class MetaObjectSerializer: public ISerializer
    {
    public:
        explicit MetaObjectSerializer(T &object)
            :object(object), unusedFields(prepareUnusedFields())
        {}

        Context::Type contextType() const override
        {
            return Context::TYPE_NAME;
        }

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

        bool allFieldsUsed() const
        {
            return unusedFields.empty();
        }
        bool isOpenObject() const
        {
            return static_cast<bool>(MetaObjectTrait<T>::attributes() &
                    static_cast<AttrMask>(ClassAttribute::OPEN));
        }

    private:
        static std::unordered_set<std::string> prepareUnusedFields()
        {
            std::unordered_set<std::string> result;
            const auto& fields = MetaObjectTrait<T>::fields();
            for(const auto &field : fields)
            {
                if(!static_cast<bool>(field.second->attributes() &
                            static_cast<AttrMask>(FieldAttribute::OPTIONAL)))
                {
                    result.insert(field.first);
                }
            }
            return result;
        }

    private:
        T &object;
        std::unordered_set<std::string> unusedFields;
    };

    template<typename T>
    template<typename V>
    void MetaObjectSerializer<T>::writeValue(const Context &context, const V &value)
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
                    throw exception::TypeSerializerException(context,
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
                throw exception::TypeSerializerException(context, "invalid field write");
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
