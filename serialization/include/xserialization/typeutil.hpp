#ifndef XSERIALIZATION_TYPEUTIL_HPP
#define XSERIALIZATION_TYPEUTIL_HPP

#include <string>
#include <type_traits>
#include <utility>

#include "xserialization/serializer.hpp"
#include "xserialization/null.hpp"

namespace xserialization::typeutil
{
    template<typename T, typename = void>
    struct IsSerializationWriteable
    {
        static constexpr bool value = false;
    };
    template<typename T>
    struct IsSerializationWriteable<T,
        std::void_t<decltype(std::declval<ISerializer>().write(
                    std::declval<T>(), std::declval<Context>()))>>
    {
        static constexpr bool value = true;
    };

    template<typename T>
    struct IsSerializationTrivial
    {
        static constexpr bool value =
            (IsSerializationWriteable<T>::value &&
             !(std::is_same_v<std::decay_t<T>, Null> ||
                 std::is_base_of_v<IDeserializer, std::decay_t<T>>));
    };

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
    using WriteType =
        std::conditional_t<IsSerializationWriteable<T>::value,
            std::conditional_t<std::is_trivial_v<std::decay_t<T>> ||
                    std::is_same_v<std::decay_t<T>, Null>, T,
                std::conditional_t<
                        std::is_base_of_v<IDeserializer, std::decay_t<T>>,
                            const IDeserializer&, const T&>>,
            const IDeserializer&>;
}

#endif
