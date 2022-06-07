#ifndef SERIALIZATION_TYPEUTIL_HPP
#define SERIALIZATION_TYPEUTIL_HPP

#include <type_traits>
#include <utility>

#include "serialization/serializer.hpp"

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
             !std::is_base_of_v<IDeserializer, std::decay_t<T>>);
    };

    template<typename T>
    using WriteType =
        std::conditional_t<IsSerializationWriteable<T>::value,
            std::conditional_t<std::is_trivial_v<std::decay_t<T>>, T,
                std::conditional_t<
                        std::is_base_of_v<IDeserializer, std::decay_t<T>>,
                            const IDeserializer&, const T&>>,
            const IDeserializer&>;
}

#endif
