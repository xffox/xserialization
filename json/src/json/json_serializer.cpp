#include "xserialization/json/json_serializer.hpp"

#include <cassert>

#include "xserialization/exception/serializer_exception.hpp"

namespace xserialization::json
{
    Context::Type JSONSerializer::contextType() const
    {
        if(value.is_object())
        {
            return Context::TYPE_NAME;
        }
        if(value.is_array())
        {
            return Context::TYPE_INDEX;
        }
        return Context::TYPE_NONE;
    }

    void JSONSerializer::write(const IDeserializer &value, const Context &context)
    {
        auto &cur = get(context);
        switch(value.contextType())
        {
            case Context::TYPE_NAME:
                cur = nlohmann::json(nlohmann::json::value_t::object);
                break;
            case Context::TYPE_INDEX:
                cur = nlohmann::json(nlohmann::json::value_t::array);
                break;
            case Context::TYPE_NONE:
                cur.clear();
                break;
            default:
                assert(false);
                break;
        }
        JSONSerializer s(cur);
        value.visit(s);
    }

    void JSONSerializer::write(Null, const Context &context)
    {
        get(context).clear();
    }

    nlohmann::json &JSONSerializer::get(const Context &context)
    {
        try
        {
            switch(context.getType())
            {
                case Context::TYPE_NAME:
                    return value[context.getName()];
                case Context::TYPE_INDEX:
                    return value[context.getIndex()];
                case Context::TYPE_NONE:
                    return value;
                default:
                    assert(false);
                    return value;
            }
        }
        catch(const nlohmann::json::exception &exc)
        {
            throw exception::SerializerException(context, "invalid context");
        }
    }
}
