#ifndef XSERIALIZATION_JSON_JSONTRAIT_HPP
#define XSERIALIZATION_JSON_JSONTRAIT_HPP

#include <xserialization/serialization_trait.hpp>

#include "xserialization/json/json.hpp"
#include "xserialization/json/json_serializer.hpp"
#include "xserialization/json/json_deserializer.hpp"

template<>
struct xserialization::SerializationTrait<xserialization::json::JSON>
{
    using Serializer = xserialization::json::JSONSerializer;

    static Serializer toSerializer(json::JSON &value)
    {
        return value.serializer();
    }
};

template<>
struct xserialization::DeserializationTrait<xserialization::json::JSON>
{
    using Deserializer = xserialization::json::JSONDeserializer;

    static Deserializer toDeserializer(const json::JSON &value)
    {
        return value.deserializer();
    }
};

#endif
