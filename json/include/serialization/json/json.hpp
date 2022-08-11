#ifndef SERIALIZATION_JSON_JSON_HPP
#define SERIALIZATION_JSON_JSON_HPP

#include <string>

#include <nlohmann/json.hpp>

#include "serialization/json/json_serializer.hpp"
#include "serialization/json/json_deserializer.hpp"

namespace serialization::json
{
    class JSON
    {
    public:
        JSON();
        JSON(const std::string &str);

        operator std::string() const;

        JSONSerializer serializer();
        JSONDeserializer deserializer() const;

    private:
        nlohmann::json value;
    };
}

#endif
