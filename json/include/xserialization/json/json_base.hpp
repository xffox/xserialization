#ifndef XSERIALIZATION_JSON_JSONBASE_HPP
#define XSERIALIZATION_JSON_JSONBASE_HPP

#include <type_traits>

#include <nlohmann/json.hpp>

#include <xserialization/context.hpp>

namespace xserialization::json
{
    template<typename Value, typename Super,
        typename = std::enable_if_t<std::is_same_v<std::remove_const_t<Value>,
            nlohmann::json>>>
    class JSONBase: public Super
    {
    public:
        [[nodiscard]]
        Context::Type contextType() const override
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

    protected:
        JSONBase(Value &value)
            :value(value)
        {}

    protected:
        Value &value;
    };
}

#endif
