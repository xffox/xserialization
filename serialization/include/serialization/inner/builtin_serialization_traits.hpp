#ifndef SERIALIZATION_SERIALIZERS_HPP
#define SERIALIZATION_SERIALIZERS_HPP

#include <vector>
#include <type_traits>

#include "serialization/serialization_trait.hpp"
#include "serialization/inner/meta_object.hpp"
#include "serialization/inner/meta_object_serializer.hpp"
#include "serialization/inner/meta_object_deserializer.hpp"

template<typename T>
struct serialization::SerializationTrait<T,
    std::enable_if_t<std::is_base_of_v<serialization::inner::MetaObject, T>>>
{
    using Serializer = serialization::inner::MetaObjectSerializer;

    static Serializer toSerializer(T &value)
    {
        return Serializer(value);
    }
};

template<typename T>
struct serialization::DeserializationTrait<T,
    std::enable_if_t<std::is_base_of_v<serialization::inner::MetaObject, T>>>
{
    using Deserializer = serialization::inner::MetaObjectDeserializer;

    static Deserializer toDeserializer(const T &value)
    {
        return Deserializer(value);
    }
};

#include "serialization/inner/collection_serializer.hpp"
#include "serialization/inner/collection_deserializer.hpp"
#include "serialization/inner/dict_serializer.hpp"
#include "serialization/inner/dict_deserializer.hpp"
#include "serialization/inner/collection_trait.hpp"
#include "serialization/inner/dict_trait.hpp"

template<typename T>
struct serialization::SerializationTrait<T,
    std::enable_if_t<serialization::inner::IsCollectionType<T>>>
{
    using Serializer = serialization::inner::CollectionSerializer<T>;

    static Serializer toSerializer(T &value)
    {
        return Serializer(value);
    }
};

template<typename T>
struct serialization::DeserializationTrait<T,
    std::enable_if_t<serialization::inner::IsCollectionType<T>>>
{
    using Deserializer = serialization::inner::CollectionDeserializer<T>;

    static Deserializer toDeserializer(const T &value)
    {
        return Deserializer(value);
    }
};

template<typename T>
struct serialization::SerializationTrait<T,
    std::enable_if_t<serialization::inner::IsDictType<T>>>
{
    using Serializer = serialization::inner::DictSerializer<T>;

    static Serializer toSerializer(T &value)
    {
        return Serializer(value);
    }
};

template<typename T>
struct serialization::DeserializationTrait<T,
    std::enable_if_t<serialization::inner::IsDictType<T>>>
{
    using Deserializer = serialization::inner::DictDeserializer<T>;

    static Deserializer toDeserializer(const T &value)
    {
        return Deserializer(value);
    }
};

#endif
