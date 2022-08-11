#ifndef XSERIALIZATION_METAANNOTATION_HPP
#define XSERIALIZATION_METAANNOTATION_HPP

#include <type_traits>
#include <string>

#include "xserialization/inner/meta_object.hpp"
#include "xserialization/typeutil.hpp"
#include "xserialization/util.hpp"
#include "xserialization/inner/attribute.hpp"

namespace xserialization::inner
{
    template<typename T, FieldAttribute... attrs>
    using FieldBase = std::conditional_t<
        (false || ... || (attrs == FieldAttribute::WEAK)),
        xserialization::inner::WeakField<T>, xserialization::inner::IField>;

    template<typename T, FieldAttribute...>
    using FieldType = T;

    template<auto... attrs>
    constexpr AttrMask attributesMask()
    {
        return (static_cast<AttrMask>(0) | ... | static_cast<AttrMask>(attrs));
    }

    template<auto...>
    struct ValList;
    template<auto V, auto... Vs>
    struct ValList<V, Vs...>
    {
        template<template<auto...> class Container, auto... Rs>
        using Init = typename ValList<Vs...>::template Init<Container, Rs..., V>;
    };
    template<auto V>
    struct ValList<V>
    {
        template<template<auto...> class Container, auto... Rs>
        using Init = Container<Rs...>;
    };
    template<>
    struct ValList<>
    {
        template<template<auto...> class Container, auto... Rs>
        using Init = Container<Rs...>;
    };

    template<ClassAttribute... attrs>
    struct ClassAttributesMaskContainer
    {
        static constexpr auto VALUE = attributesMask<attrs...>();
    };

    template<typename, FieldAttribute... attrs>
    constexpr AttrMask fieldAttributesMaskFromTail()
    {
        return attributesMask<attrs...>();
    }

    template<auto... attrs>
    constexpr AttrMask classAttributesMaskFromInit()
    {
        return ValList<attrs...>::template Init<
            ClassAttributesMaskContainer>::VALUE;
    }
}

inline constexpr auto MT_FIELD_ATTR_WEAK =
    xserialization::inner::FieldAttribute::WEAK;
inline constexpr auto MT_FIELD_ATTR_OPTIONAL =
    xserialization::inner::FieldAttribute::OPTIONAL;

inline constexpr auto MT_CLASS_ATTR_OPEN =
    xserialization::inner::ClassAttribute::OPEN;

#define MT_INNER_FIELD_DEF(name, ...) \
    struct _##name##FieldInitializerType \
    { \
        _##name##FieldInitializerType() \
        { \
            static _##name##FieldType field; \
        } \
    } _##name##FieldInitializer; \
    xserialization::inner::FieldType<__VA_ARGS__> name

#define MT_FIELD(name, ...) \
    struct _##name##FieldType: public xserialization::inner::FieldBase<__VA_ARGS__> \
    { \
        using xserialization::inner::FieldBase<__VA_ARGS__>::write; \
        _##name##FieldType() \
        { \
            BaseClass::addField(#name, *this); \
        } \
        void visit(xserialization::ISerializer &serializer, \
            const xserialization::inner::MetaObject &object) override \
        { \
            const auto &cur = static_cast<const BaseClass&>(object); \
            const auto &value = cur.name; \
            const xserialization::Context context(#name); \
            xserialization::util::visitValue(serializer, value, context); \
        } \
        bool write(xserialization::inner::MetaObject &object, \
                xserialization::typeutil::WriteType< \
                    xserialization::inner::FieldType<__VA_ARGS__>> value) override \
        { \
            auto &cur = static_cast<BaseClass&>(object); \
            return xserialization::util::writeValue(cur.name, value); \
        } \
        xserialization::inner::AttrMask attributes() const override \
        { \
            return xserialization::inner::fieldAttributesMaskFromTail<__VA_ARGS__>(); \
        } \
    }; \
    MT_INNER_FIELD_DEF(name, __VA_ARGS__)

#define MT_INNER_CLASS(cl, ...) \
    class cl; \
    class _##cl##ClassBase: public xserialization::inner::MetaObjectBase<cl, \
        xserialization::inner::classAttributesMaskFromInit<__VA_ARGS__>()> \
    { \
    public: \
        const std::string &className() const override \
        { \
            static const std::string name(#cl); \
            return name; \
        } \
    }; \
    class cl: public _##cl##ClassBase

#define MT_CLASS(...) MT_INNER_CLASS(__VA_ARGS__, 0)

#endif
