#ifndef SERIALIZATION_JSON_JSONTRAIT_HPP
#define SERIALIZATION_JSON_JSONTRAIT_HPP

#include <serialization/serialization_trait.hpp>

#include "serialization/json/json.hpp"
#include "serialization/json/json_serializer.hpp"
#include "serialization/json/json_deserializer.hpp"

template<>
struct serialization::SerializationTrait<serialization::json::JSON>
{
    using Serializer = serialization::json::JSONSerializer;

    static Serializer toSerializer(json::JSON &value)
    {
        return value.serializer();
    }
};

template<>
struct serialization::DeserializationTrait<serialization::json::JSON>
{
    using Deserializer = serialization::json::JSONDeserializer;

    static Deserializer toDeserializer(const json::JSON &value)
    {
        return value.deserializer();
    }
};

#endif
