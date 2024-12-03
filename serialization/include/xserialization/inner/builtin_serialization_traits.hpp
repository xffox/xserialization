#ifndef XSERIALIZATION_SERIALIZERS_HPP
#define XSERIALIZATION_SERIALIZERS_HPP

#include <vector>
#include <string>
#include <type_traits>

#include "xserialization/serialization_trait.hpp"
#include "xserialization/inner/meta_object.hpp"
#include "xserialization/inner/meta_object_serializer.hpp"
#include "xserialization/inner/meta_object_deserializer.hpp"

template<typename T>
struct xserialization::SerializationTrait<T,
    std::void_t<xserialization::inner::MetaObjectTrait<T>>>
{
    using Serializer = xserialization::inner::MetaObjectSerializer<T>;

    static Serializer toSerializer(T &value)
    {
        return Serializer(value);
    }
};

template<typename T>
struct xserialization::DeserializationTrait<T,
    std::void_t<xserialization::inner::MetaObjectTrait<T>>>
{
    using Deserializer = xserialization::inner::MetaObjectDeserializer<T>;

    static Deserializer toDeserializer(const T &value)
    {
        return Deserializer(value);
    }
};

#include "xserialization/inner/collection_serializer.hpp"
#include "xserialization/inner/collection_deserializer.hpp"
#include "xserialization/inner/dict_serializer.hpp"
#include "xserialization/inner/dict_deserializer.hpp"
#include "xserialization/inner/collection_trait.hpp"
#include "xserialization/inner/dict_trait.hpp"
#include "xserialization/inner/atom_serializer.hpp"
#include "xserialization/inner/atom_deserializer.hpp"
#include "xserialization/typeutil.hpp"

template<typename T>
struct xserialization::SerializationTrait<T,
    std::enable_if_t<xserialization::inner::IsCollectionType<T> && !std::is_same_v<T, std::string>>>
{
    using Serializer = xserialization::inner::CollectionSerializer<T>;

    static Serializer toSerializer(T &value)
    {
        return Serializer(value);
    }
};

template<typename T>
struct xserialization::DeserializationTrait<T,
    std::enable_if_t<xserialization::inner::IsCollectionType<T> && !std::is_same_v<T, std::string>>>
{
    using Deserializer = xserialization::inner::CollectionDeserializer<T>;

    static Deserializer toDeserializer(const T &value)
    {
        return Deserializer(value);
    }
};

template<typename T>
struct xserialization::SerializationTrait<T,
    std::enable_if_t<xserialization::inner::IsDictType<T>>>
{
    using Serializer = xserialization::inner::DictSerializer<T>;

    static Serializer toSerializer(T &value)
    {
        return Serializer(value);
    }
};

template<typename T>
struct xserialization::DeserializationTrait<T,
    std::enable_if_t<xserialization::inner::IsDictType<T>>>
{
    using Deserializer = xserialization::inner::DictDeserializer<T>;

    static Deserializer toDeserializer(const T &value)
    {
        return Deserializer(value);
    }
};

template<typename T>
struct xserialization::SerializationTrait<T,
    std::enable_if_t<xserialization::typeutil::IsSerializationTrivial<T>::value>>
{
    using Serializer = xserialization::inner::AtomSerializer<T>;

    static Serializer toSerializer(T &value)
    {
        return Serializer(value);
    }
};

template<typename T>
struct xserialization::DeserializationTrait<T,
    std::enable_if_t<xserialization::typeutil::IsSerializationTrivial<T>::value>>
{
    using Deserializer = xserialization::inner::AtomDeserializer<T>;

    static Deserializer toDeserializer(const T &value)
    {
        return Deserializer(value);
    }
};

#endif
