#include "xserialization/json/json_deserializer.hpp"

#include <iterator>
#include <optional>
#include <cassert>

#include <nlohmann/json.hpp>

#include "xserialization/context.hpp"
#include "xserialization/serializer.hpp"
#include "xserialization/null.hpp"
#include "xserialization/exception/serialization_exception.hpp"

namespace xserialization::json
{
    namespace
    {
        template<typename F, typename E>
        decltype(auto) tryConsumeValue(const nlohmann::json &value, F consume,
                E invalid)
        {
            switch(value.type())
            {
            case nlohmann::json::value_t::boolean:
                return consume(value.get<nlohmann::json::boolean_t>());
            case nlohmann::json::value_t::number_integer:
                return consume(value.get<nlohmann::json::number_integer_t>());
            case nlohmann::json::value_t::number_unsigned:
                return consume(value.get<nlohmann::json::number_unsigned_t>());
            case nlohmann::json::value_t::number_float:
                return consume(value.get<nlohmann::json::number_float_t>());
            case nlohmann::json::value_t::string:
                return consume(value.get<nlohmann::json::string_t>());
            case nlohmann::json::value_t::null:
                return consume(Null{});
            default:
                return invalid();
            }
        }
    }

    void JSONDeserializer::visit(ISerializer &serializer) const
    {
        auto writeValue =
            [&serializer](const nlohmann::json &value, const Context &context){
                tryConsumeValue(value, [&serializer, &context](const auto &v){
                            serializer.write(v, context);
                        },
                        [&serializer, &context, &value](){
                            serializer.write(JSONDeserializer(value), context);
                        });
        };
        switch(contextType())
        {
        case Context::TYPE_NAME:
            for(const auto &el : value.items())
            {
                writeValue(el.value(), el.key());
            }
            break;
        case Context::TYPE_INDEX:
            for(auto iter = std::begin(value); iter != std::end(value); ++iter)
            {
                writeValue(*iter, iter-std::begin(value));
            }
            break;
        case Context::TYPE_NONE:
            writeValue(value, {});
            break;
        default:
            assert(false);
            break;
        }
    }
}
