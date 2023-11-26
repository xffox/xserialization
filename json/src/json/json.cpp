#include "xserialization/json/json.hpp"

#include <string>

#include <nlohmann/json.hpp>

#include <xserialization/exception/serializer_exception.hpp>

#include "json/util.hpp"

namespace xserialization::json
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

    struct JSON::Value
    {
        nlohmann::json value;
    };

    JSON::JSON()
        :value(new Value{})
    {}

    JSON::JSON(const std::string &str)
        :value(new Value{parse(str)})
    {}

    JSON::JSON(const JSON &that)
        :value(new Value{util::assertHasValue(that.value)->value})
    {}

    JSON::~JSON() = default;

    JSON &JSON::operator=(const JSON &that)
    {
        if(this == &that)
        {
            return *this;
        }
        value.reset(new Value{util::assertHasValue(that.value)->value});
        return *this;
    }

    JSON::operator std::string() const
    {
        return util::assertHasValue(value)->value.dump();
    }

    JSONSerializer JSON::serializer()
    {
        return JSONSerializer{util::assertHasValue(value)->value};
    }

    JSONDeserializer JSON::deserializer() const
    {
        return JSONDeserializer{util::assertHasValue(value)->value};
    }
}
