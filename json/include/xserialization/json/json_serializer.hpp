#ifndef XSERIALIZATION_JSON_JSONSERIALIZER_HPP
#define XSERIALIZATION_JSON_JSONSERIALIZER_HPP

#include "xserialization/serializer.hpp"
#include "xserialization/json/inner/json_base.hpp"
#include "xserialization/json/inner/opaque_json.hpp"

namespace xserialization::json
{
    class JSONSerializer: public inner::JSONBase<inner::OpaqueJSON, ISerializer>
    {
        friend class JSON;
    public:
        using JSONBase::JSONBase;

        void write(const IDeserializer &value, const Context &context) override;
        void write(Null, const Context &context) override;

        void write(bool value, const Context &context) override;
        void write(char value, const Context &context) override;
        void write(signed char value, const Context &context) override;
        void write(unsigned char value, const Context &context) override;
        void write(short value, const Context &context) override;
        void write(unsigned short value, const Context &context) override;
        void write(int value, const Context &context) override;
        void write(unsigned int value, const Context &context) override;
        void write(long value, const Context &context) override;
        void write(unsigned long value, const Context &context) override;
        void write(long long value, const Context &context) override;
        void write(unsigned long long value, const Context &context) override;
        void write(float value, const Context &context) override;
        void write(double value, const Context &context) override;
        void write(long double value, const Context &context) override;
        void write(const std::string &value, const Context &context) override;
    };
}

#endif
