#ifndef XSERIALIZATION_JSON_INNER_OPAQUEJSON_HPP
#define XSERIALIZATION_JSON_INNER_OPAQUEJSON_HPP

#include "xserialization/context.hpp"

namespace xserialization::json::inner
{
    using OpaqueJSON = void;

    Context::Type contextType(const OpaqueJSON *json);
}

#endif
