#ifndef XSERIALIZATION_METAANNOTATION_HPP
#define XSERIALIZATION_METAANNOTATION_HPP

#include <type_traits>
#include <string>
#include <cassert>

#include "xserialization/inner/meta_object.hpp"
#include "xserialization/inner/meta_field.hpp"
#include "xserialization/typeutil.hpp"
#include "xserialization/util.hpp"
#include "xserialization/inner/attribute.hpp"

namespace xserialization::inner
{
    template<typename Cl, typename T, FieldAttribute... attrs>
    using FieldBase = std::conditional_t<
        (false || ... || (attrs == FieldAttribute::WEAK)),
        xserialization::inner::field::WeakField<Cl, T>, \
            xserialization::inner::field::IField<Cl>>;

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

#define MT_INNER_FIELD(name, curclass, ...) \
    static inline const struct _##name##FieldInitializerType: protected xserialization::inner::FieldBase<curclass, __VA_ARGS__> \
    { \
        using xserialization::inner::FieldBase<curclass, __VA_ARGS__>::write; \
        _##name##FieldInitializerType() noexcept \
        { \
            [[maybe_unused]] const auto res = curclass::addField(#name, *this); \
            assert(res); \
        } \
        void visit(xserialization::ISerializer &serializer, \
            const curclass &cur) override final \
        { \
            const auto &value = cur.name; \
            const xserialization::Context context(#name); \
            xserialization::util::visitValue(serializer, value, context); \
        } \
        bool write(curclass &cur, \
                xserialization::typeutil::WriteType< \
                    xserialization::inner::FieldType<__VA_ARGS__>> value) override final \
        { \
            return xserialization::util::writeValue(cur.name, value); \
        } \
        xserialization::inner::AttrMask attributes() const override final \
        { \
            return xserialization::inner::fieldAttributesMaskFromTail<__VA_ARGS__>(); \
        } \
    } _##name##FieldInitializer; \
    std::enable_if_t<&_##name##FieldInitializer != nullptr, xserialization::inner::FieldType<__VA_ARGS__>> name

#define MT_FIELD(name, ...) MT_INNER_FIELD(name, CurClass, __VA_ARGS__)

// For templated types current rules require specifying the current class
// explicitly when declaring fields.
#define MT_FIELD_IN(curclass, name, ...) MT_INNER_FIELD(name, curclass, __VA_ARGS__)

#define MT_INNER_CLASS(kind, cl, ...) \
    kind cl; \
    class _##cl##ClassBase: public xserialization::inner::object::MetaObjectBase<cl, \
        xserialization::inner::classAttributesMaskFromInit<__VA_ARGS__>()> \
    { \
    protected: \
        using MetaObjectBase = xserialization::inner::object::MetaObjectBase<cl, xserialization::inner::classAttributesMaskFromInit<__VA_ARGS__>()>; \
    public: \
        using MetaObjectBase::BaseClass::attributes; \
        using MetaObjectBase::BaseClass::fields; \
        const std::string &className() const \
        { \
            static const std::string name(#cl); \
            return name; \
        } \
    }; \
    kind cl: public _##cl##ClassBase

#define MT_INNER_TEMPLATE_CLASS(kind, cl, typearg, ...) \
    template<typename typearg> \
    kind cl; \
    template<typename typearg> \
    class _##cl##ClassBase: public xserialization::inner::object::MetaObjectBase<cl<typearg>, \
        xserialization::inner::classAttributesMaskFromInit<__VA_ARGS__>()> \
    { \
    protected: \
        using MetaObjectBase = xserialization::inner::object::MetaObjectBase<cl<typearg>, xserialization::inner::classAttributesMaskFromInit<__VA_ARGS__>()>; \
    public: \
        using MetaObjectBase::BaseClass::attributes; \
        using MetaObjectBase::BaseClass::fields; \
        const std::string &className() const \
        { \
            static const std::string name(#cl); \
            return name; \
        } \
    }; \
    template<typename typearg> \
    kind cl: public _##cl##ClassBase<typearg>

#define MT_CLASS(...) MT_INNER_CLASS(class, __VA_ARGS__, 0)
#define MT_STRUCT(...) MT_INNER_CLASS(struct, __VA_ARGS__, 0)

#define MT_TEMPLATE_CLASS(...) MT_INNER_TEMPLATE_CLASS(class, __VA_ARGS__, 0)
#define MT_TEMPLATE_STRUCT(...) MT_INNER_TEMPLATE_CLASS(struct, __VA_ARGS__, 0)

#endif
