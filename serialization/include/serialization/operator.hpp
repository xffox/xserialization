#ifndef SERIALIZATION_OPERATOR_HPP
#define SERIALIZATION_OPERATOR_HPP

#include <tuple>
#include <type_traits>

#include "serialization/context.hpp"
#include "serialization/serializer.hpp"
#include "serialization/deserializer.hpp"
#include "serialization/serialization_trait.hpp"

namespace serialization
{
    template<typename T, typename Context>
    void operator<<(serialization::ISerializer &serializer,
        const std::tuple<T, Context> &contextedObject)
    {
        const auto &[object, context] = contextedObject;
        serializer.write(
                serialization::DeserializationTrait<std::decay_t<T>>::toDeserializer(object),
                context);
    }

    template<typename T>
    void operator<<(serialization::ISerializer &serializer,
        const T &object)
    {
        serializer<<std::forward_as_tuple(object, serialization::Context());
    }

    template<typename T>
    void operator>>(const serialization::IDeserializer &deserializer, T &object)
    {
        serialization::SerializationTrait<T>::toSerializer(object).write(deserializer,
                serialization::Context());
    }
}

#endif
