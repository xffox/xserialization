#ifndef SERIALIZATION_OPERATOR_HPP
#define SERIALIZATION_OPERATOR_HPP

#include <tuple>

#include "serialization/context.hpp"
#include "serialization/serializer.hpp"
#include "serialization/deserializer.hpp"

namespace serialization
{
    template<typename T, typename Context>
    void operator<<(ISerializer &serializer,
        const std::tuple<T, Context> &contextedObject);
    template<typename T>
    void operator<<(ISerializer &serializer, const T &object);
    template<typename T>
    void operator>>(const IDeserializer &deserializer, T &object);

    template<typename T>
    decltype(auto) toSerializer(T &&value);
    template<typename T>
    decltype(auto) toDeserializer(T &&value);
}

#include <type_traits>
#include <utility>

#include "serialization/serialization_trait.hpp"

namespace serialization
{
    template<typename T>
    decltype(auto) toSerializer(T &&value)
    {
        return SerializationTrait<std::decay_t<T>>::toSerializer(std::forward<T>(value));
    }

    template<typename T>
    decltype(auto) toDeserializer(T &&value)
    {
        return DeserializationTrait<std::decay_t<T>>::toDeserializer(std::forward<T>(value));
    }

    template<typename T, typename Context>
    void operator<<(ISerializer &serializer,
        const std::tuple<T, Context> &contextedObject)
    {
        const auto &[object, context] = contextedObject;
        serializer.write(toDeserializer(object), context);
    }

    template<typename T>
    void operator<<(ISerializer &serializer, const T &object)
    {
        serializer<<std::forward_as_tuple(object, Context());
    }

    template<typename T>
    void operator>>(const IDeserializer &deserializer, T &object)
    {
        toSerializer(object).write(deserializer, Context());
    }
}

#endif
