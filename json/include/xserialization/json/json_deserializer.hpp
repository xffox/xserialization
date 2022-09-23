#ifndef XSERIALIZATION_JSON_JSONDESERIALIZER_HPP
#define XSERIALIZATION_JSON_JSONDESERIALIZER_HPP

#include <nlohmann/json.hpp>

#include <xserialization/deserializer.hpp>
#include "xserialization/json/json_base.hpp"

namespace xserialization::json
{
    class JSONDeserializer: public JSONBase<const nlohmann::json, IDeserializer>
    {
        friend class JSON;
    public:
        void visit(ISerializer &serializer) const override;

    private:
        explicit JSONDeserializer(const nlohmann::json &value)
            :JSONBase(value)
        {}
    };
}

#endif
