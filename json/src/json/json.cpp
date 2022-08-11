#include "serialization/json/json.hpp"

#include <string>

#include <nlohmann/json.hpp>

#include <serialization/exception/serializer_exception.hpp>

namespace serialization::json
{
    namespace
    {
        nlohmann::json parse(const std::string &str)
        {
            try
            {
                return nlohmann::json::parse(str);
            }
            catch(const nlohmann::json::exception &exc)
            {
                throw exception::SerializerException(Context(),
                        exc.what());
            }
        }
    }

    JSON::JSON()
        :value{}
    {}

    JSON::JSON(const std::string &str)
        :value(parse(str))
    {}

    JSON::operator std::string() const
    {
        return value.dump();
    }

    JSONSerializer JSON::serializer()
    {
        return JSONSerializer(value);
    }

    JSONDeserializer JSON::deserializer() const
    {
        return JSONDeserializer(value);
    }
}
