#ifndef XSERIALIZATION_JSON_JSONDESERIALIZER_HPP
#define XSERIALIZATION_JSON_JSONDESERIALIZER_HPP

#include "xserialization/deserializer.hpp"
#include "xserialization/json/inner/json_base.hpp"
#include "xserialization/json/inner/opaque_json.hpp"

namespace xserialization::json
{
    class JSONDeserializer:
        public inner::JSONBase<const inner::OpaqueJSON, IDeserializer>
    {
        friend class JSON;
    public:
        using JSONBase::JSONBase;

        void visit(ISerializer &serializer) const override;
    };
}

#endif
