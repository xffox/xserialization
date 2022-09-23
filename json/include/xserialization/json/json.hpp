#ifndef XSERIALIZATION_JSON_JSON_HPP
#define XSERIALIZATION_JSON_JSON_HPP

#include <string>

#include <nlohmann/json.hpp>

#include "xserialization/json/json_serializer.hpp"
#include "xserialization/json/json_deserializer.hpp"

namespace xserialization::json
{
    class JSON
    {
    public:
        JSON();
        JSON(const std::string &str);

        operator std::string() const;

        [[nodiscard]]
        JSONSerializer serializer();
        [[nodiscard]]
        JSONDeserializer deserializer() const;

    private:
        nlohmann::json value;
    };
}

#endif
