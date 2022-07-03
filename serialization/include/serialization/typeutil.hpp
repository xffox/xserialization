#ifndef SERIALIZATION_TYPEUTIL_HPP
#define SERIALIZATION_TYPEUTIL_HPP

#include <string>
#include <type_traits>
#include <utility>

#include "serialization/serializer.hpp"
#include "serialization/null.hpp"

namespace serialization::typeutil
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

    template<template<typename...> class Container>
    struct SerializationTrivialTypes
    {
        using Type = Container<bool,
              char, signed char, unsigned char,
              short, unsigned short,
              int, unsigned int,
              long, unsigned long,
              long long, unsigned long long,
              float, double, long double,
              std::string>;
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
