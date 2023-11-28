#ifndef XSERIALIZATION_JSON_INNER_JSONBASE_HPP
#define XSERIALIZATION_JSON_INNER_JSONBASE_HPP

#include <type_traits>

#include "xserialization/context.hpp"
#include "xserialization/json/inner/opaque_json.hpp"

namespace xserialization::json::inner
{
    template<typename OpaqueType, typename Super,
        typename = std::enable_if_t<std::is_same_v<
            std::remove_const_t<OpaqueType>, OpaqueJSON>>>
    class JSONBase: public Super
    {
    public:
        [[nodiscard]]
        Context::Type contextType() const override
        {
            return inner::contextType(json);
        }

    protected:
        explicit JSONBase(OpaqueType *json)
            :json(json)
        {}

    protected:
        OpaqueType *json;
    };
}

#endif
