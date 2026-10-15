#ifndef XSERIALIZATION_INNER_METAOBJECT_HPP
#define XSERIALIZATION_INNER_METAOBJECT_HPP

#include <type_traits>
#include <string>
#include <unordered_map>
#include <cstdlib>
#include <utility>

#include "xserialization/inner/attribute.hpp"

namespace xserialization::inner
{
    namespace field
    {
        class IField;
    };

    namespace object
    {
        class MetaObject
        {
        public:
            using FieldMap = std::unordered_map<std::string, field::IField*>;
            using Attrs = AttrMask;
        };

        template<class T, AttrMask classAttributes>
        class MetaObjectBase: public MetaObject
        {
        protected:
            using CurClass = T;
            using BaseClass = MetaObjectBase;

        public:
            [[nodiscard]]
            static const FieldMap &fields() noexcept
            {
                return MetaObjectBase::getFields();
            }

            [[nodiscard]]
            static AttrMask attributes() noexcept
            {
                return classAttributes;
            }

        protected:
            static bool addField(const std::string &name, field::IField &field) noexcept
            {
                try
                {
                    return getFields().insert(std::make_pair(name, &field)).second;
                }
                catch(...)
                {
                    std::abort();
                }
            }

        private:
            static FieldMap &getFields() noexcept
            {
                try
                {
                    static FieldMap fields;
                    return fields;
                }
                catch(...)
                {
                    abort();
                }
            }
        };

        template<typename T, typename = void>
        struct MetaObjectTrait;
        template<typename T>
        struct MetaObjectTrait<T,
            std::enable_if_t<std::is_base_of_v<MetaObject, T>>>
        {
            using TYPE = T;
        };
    }

    template<typename T>
    using MetaObjectTrait = typename object::MetaObjectTrait<T>::TYPE;
}

#endif
