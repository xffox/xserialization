#include "xserialization/json/inner/opaque_json.hpp"

#include "json_impl.hpp"

namespace xserialization::json::inner
{
    Context::Type contextType(const OpaqueJSON *opaque_json)
    {
        const auto &json = toJSONImpl(opaque_json);
        if(json.is_object())
        {
            return Context::TYPE_NAME;
        }
        if(json.is_array())
        {
            return Context::TYPE_INDEX;
        }
        return Context::TYPE_NONE;
    }
}
