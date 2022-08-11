#ifndef XSERIALIZATION_JSON_JSONDESERIALIZER_HPP
#define XSERIALIZATION_JSON_JSONDESERIALIZER_HPP

#include <nlohmann/json.hpp>

#include <xserialization/deserializer.hpp>

namespace xserialization::json
{
    class JSON;

    class JSONDeserializer: public IDeserializer
    {
        friend JSON;
    public:
        Context::Type contextType() const override;

        void visit(ISerializer &serializer) const override;

    private:
        JSONDeserializer(const nlohmann::json &value)
            :value(value)
        {}

    private:
        const nlohmann::json &value;
    };
}

#endif
