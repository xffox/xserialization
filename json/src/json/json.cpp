#include "xserialization/json/json.hpp"

#include <string>

#include "xserialization/exception/serializer_exception.hpp"
#include "json/util.hpp"
#include "inner/json_impl.hpp"

namespace xserialization::json
{
    namespace
    {
        inner::JSONImpl parse(const std::string &str)
        {
            try
            {
                return inner::JSONImpl::parse(str);
            }
            catch(const inner::JSONImpl::exception &exc)
            {
                throw exception::SerializerException(Context(),
                        exc.what());
            }
        }
    }

    struct JSON::Value
    {
        inner::JSONImpl value;
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
        return JSONSerializer{
            inner::toJSONOpaque(util::assertHasValue(value)->value)};
    }

    JSONDeserializer JSON::deserializer() const
    {
        return JSONDeserializer{
            inner::toJSONOpaque(util::assertHasValue(value)->value)};
    }
}
