#ifndef SERIALIZATION_SERIALIZERS_HPP
#define SERIALIZATION_SERIALIZERS_HPP

#include <vector>
#include <type_traits>

#include "serialization/serialization_trait.hpp"
#include "serialization/inner/meta_object.hpp"

template<typename T>
struct serialization::SerializationTrait<T,
    std::enable_if_t<std::is_base_of_v<serialization::inner::MetaObject, T>>>
{
    using Serializer = serialization::inner::MetaObject&;

    static Serializer toSerializer(T &value) {
        return value;
    }
};

template<typename T>
struct serialization::DeserializationTrait<T,
    std::enable_if_t<std::is_base_of_v<serialization::inner::MetaObject, T>>>
{
    using Deserializer = const serialization::inner::MetaObject&;

    static Deserializer toDeserializer(const T &value) {
        return value;
    }
};

#include "serialization/inner/collection_serializer.hpp"

template<typename T>
struct serialization::SerializationTrait<std::vector<T>>
{
    using Serializer = serialization::inner::CollectionSerializer<std::vector<T>>;

    static Serializer toSerializer(std::vector<T> &value) {
        return Serializer(value);
    }
};

template<typename T>
struct serialization::DeserializationTrait<std::vector<T>>
{
    using Deserializer = serialization::inner::CollectionDeserializer<std::vector<T>>;

    static Deserializer toDeserializer(const std::vector<T> &value) {
        return Deserializer(value);
    }
};

#endif
