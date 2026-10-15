#ifndef XSERIALIZATION_INNER_METAOBJECTSERIALIZER_HPP
#define XSERIALIZATION_INNER_METAOBJECTSERIALIZER_HPP

#include <string>
#include <unordered_set>

#include "xserialization/serializer.hpp"
#include "xserialization/context.hpp"
#include "xserialization/inner/meta_object.hpp"

namespace xserialization::inner
{
    class MetaObjectSerializer: public ISerializer
    {
    public:
        template<typename T>
        explicit MetaObjectSerializer(T &object)
        :MetaObjectSerializer(object, MetaObjectTrait<T>::fields(), MetaObjectTrait<T>::attributes())
        {}

        Context::Type contextType() const override;

        void write(const IDeserializer &value, const Context &context) override;
        void write(Null value, const Context &context) override;
        void write(bool value, const Context &context) override;
        void write(char value, const Context &context) override;
        void write(signed char value, const Context &context) override;
        void write(unsigned char value, const Context &context) override;
        void write(short value, const Context &context) override;
        void write(unsigned short value, const Context &context) override;
        void write(int value, const Context &context) override;
        void write(unsigned int value, const Context &context) override;
        void write(long value, const Context &context) override;
        void write(unsigned long value, const Context &context) override;
        void write(long long value, const Context &context) override;
        void write(unsigned long long value, const Context &context) override;
        void write(float value, const Context &context) override;
        void write(double value, const Context &context) override;
        void write(long double value, const Context &context) override;
        void write(const std::string &value, const Context &context) override;

    private:
        explicit MetaObjectSerializer(object::MetaObject &object,
            const object::MetaObject::FieldMap &fields,
            object::MetaObject::Attrs attrs);

        template<typename V>
        void writeValue(const Context &context, const V &value);

        const std::unordered_set<std::string> &getUnusedFields() const;
        bool isOpenObject() const;

    private:
        object::MetaObject &object;
        const object::MetaObject::FieldMap &fields_;
        object::MetaObject::Attrs attrs_;
        std::unordered_set<std::string> unusedFields;
    };
}

#endif
