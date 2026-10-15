#include "xserialization/inner/meta_object_serializer.hpp"
#include "xserialization/inner/meta_field.hpp"

#include <iterator>
#include <sstream>
#include <type_traits>
#include <utility>

#include "xserialization/exception/serializer_exception.hpp"
#include "xserialization/inner/attribute.hpp"

namespace xserialization::inner
{
    namespace 
    {
        std::unordered_set<std::string> prepareUnusedFields(
            const object::MetaObject::FieldMap &fields)
        {
            std::unordered_set<std::string> result;
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
    }

    MetaObjectSerializer::MetaObjectSerializer(object::MetaObject &object,
        const object::MetaObject::FieldMap &fields,
        object::MetaObject::Attrs attrs)
        :object(object), fields_(fields), attrs_(attrs), unusedFields(prepareUnusedFields(fields_))
    {}

    Context::Type MetaObjectSerializer::contextType() const
    {
        return Context::TYPE_NAME;
    }

    void MetaObjectSerializer::write(const IDeserializer &value, const Context &context)
    { writeValue(context, value); }
    void MetaObjectSerializer::write(Null value, const Context &context)
    { writeValue(context, value); }
    void MetaObjectSerializer::write(bool value, const Context &context)
    { writeValue(context, value); }
    void MetaObjectSerializer::write(char value, const Context &context)
    { writeValue(context, value); }
    void MetaObjectSerializer::write(signed char value, const Context &context)
    { writeValue(context, value); }
    void MetaObjectSerializer::write(unsigned char value, const Context &context)
    { writeValue(context, value); }
    void MetaObjectSerializer::write(short value, const Context &context)
    { writeValue(context, value); }
    void MetaObjectSerializer::write(unsigned short value, const Context &context)
    { writeValue(context, value); }
    void MetaObjectSerializer::write(int value, const Context &context)
    { writeValue(context, value); }
    void MetaObjectSerializer::write(unsigned int value, const Context &context)
    { writeValue(context, value); }
    void MetaObjectSerializer::write(long value, const Context &context)
    { writeValue(context, value); }
    void MetaObjectSerializer::write(unsigned long value, const Context &context)
    { writeValue(context, value); }
    void MetaObjectSerializer::write(long long value, const Context &context)
    { writeValue(context, value); }
    void MetaObjectSerializer::write(unsigned long long value, const Context &context)
    { writeValue(context, value); }
    void MetaObjectSerializer::write(float value, const Context &context)
    { writeValue(context, value); }
    void MetaObjectSerializer::write(double value, const Context &context)
    { writeValue(context, value); }
    void MetaObjectSerializer::write(long double value, const Context &context)
    { writeValue(context, value); }
    void MetaObjectSerializer::write(const std::string &value, const Context &context)
    { writeValue(context, value); }

    const std::unordered_set<std::string> &MetaObjectSerializer::getUnusedFields() const
    {
        return unusedFields;
    }
    bool MetaObjectSerializer::isOpenObject() const
    {
        return static_cast<bool>(attrs_ &
            static_cast<AttrMask>(ClassAttribute::OPEN));
    }

    template<typename V>
    void MetaObjectSerializer::writeValue(const Context &context, const V &value)
    {
        if(context.getType() == Context::TYPE_NONE)
        {
            if constexpr(std::is_same_v<std::decay_t<V>, IDeserializer>)
            {
                if(value.contextType() == Context::TYPE_NAME)
                {
                    MetaObjectSerializer serializer(object, fields_, attrs_);
                    value.visit(serializer);
                    if(!serializer.getUnusedFields().empty())
                    {
                        std::stringstream errorStream;
                        errorStream<<"missing fields: ";
                        bool first = true;
                        for(const auto &field : serializer.getUnusedFields())
                        {
                            if(!first)
                            {
                                errorStream<<',';
                            }
                            else
                            {
                                first = false;
                            }
                            errorStream<<"'"<<field<<"'";
                        }
                        throw exception::SerializerException(context,
                                std::move(errorStream).str());
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
        if(const auto iter = fields_.find(context.getName());
            iter != std::end(fields_))
        {
            iter->second->makeSerializer(&object)->write(value, Context{});
            unusedFields.erase(context.getName());
        }
        else
        {
            if(!isOpenObject())
            {
                std::stringstream errorStream;
                errorStream<<"unknown field: '"<<context.getName()<<"'";
                throw exception::SerializerException(context,
                        std::move(errorStream).str());
            }
        }
    }
}
