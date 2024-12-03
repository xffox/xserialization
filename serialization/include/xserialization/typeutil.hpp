#ifndef XSERIALIZATION_TYPEUTIL_HPP
#define XSERIALIZATION_TYPEUTIL_HPP

#include <string>
#include <type_traits>
#include <utility>

#include "xserialization/serializer.hpp"
#include "xserialization/null.hpp"

namespace xserialization::typeutil
{
    template<typename... Args>
    struct TypeList
    {
        template<typename... As>
        using Append = TypeList<Args..., As...>;

        template<template<typename...> class Container>
        using Apply = Container<Args...>;
    };

    template<template<typename...> class Container>
    struct SerializationSignedIntegerTypes
    {
        using Type = Container<signed char, short, int, long, long long>;
    };
    template<template<typename...> class Container>
    struct SerializationUnsignedIntegerTypes
    {
        using Type = Container<unsigned char, unsigned short, unsigned int,
              unsigned long, unsigned long long>;
    };
    template<template<typename...> class Container>
    struct SerializationFloatingPointTypes
    {
        using Type = Container<float, double, long double>;
    };
    template<template<typename...> class Container>
    struct SerializationTrivialTypes
    {
        using Type =
            SerializationSignedIntegerTypes<
            SerializationUnsignedIntegerTypes<
            SerializationFloatingPointTypes<
                TypeList<bool, char, std::string>::Append>
                ::Type::Append>::Type::Append>::Type::Apply<Container>;
    };

    template<typename T>
    class IsSerializationTrivial
    {
        template<typename... Args>
        struct EqType
        {
            static constexpr bool value = (std::is_same_v<T, Args> || ...);
        };
    public:
        static constexpr bool value = SerializationTrivialTypes<EqType>::Type::value;
    };

    template<typename T>
    using WriteType =
        std::conditional_t<(IsSerializationTrivial<T>::value ||
                std::is_same_v<std::decay_t<T>, Null>),
            std::conditional_t<std::is_trivial_v<std::decay_t<T>> ||
                    std::is_same_v<std::decay_t<T>, Null>, T, const T&>,
            const IDeserializer&>;
}

#endif
