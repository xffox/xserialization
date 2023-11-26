#ifndef XSERIALIZATION_JSON_JSON_HPP
#define XSERIALIZATION_JSON_JSON_HPP

#include <string>
#include <memory>

#include "xserialization/json/json_serializer.hpp"
#include "xserialization/json/json_deserializer.hpp"

namespace xserialization::json
{
    class JSON
    {
    public:
        JSON();
        JSON(const std::string &str);
        JSON(const JSON &that);
        JSON(JSON&&) = default;
        ~JSON();

        JSON &operator=(const JSON &that);
        JSON &operator=(JSON&&) = default;

        operator std::string() const;

        [[nodiscard]]
        JSONSerializer serializer();
        [[nodiscard]]
        JSONDeserializer deserializer() const;

    private:
        struct Value;
        std::unique_ptr<Value> value;
    };
}

#endif
