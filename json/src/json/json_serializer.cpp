#include "xserialization/json/json_serializer.hpp"

#include <cassert>

#include "xserialization/exception/serializer_exception.hpp"
#include "xserialization/json/inner/opaque_json.hpp"
#include "inner/json_impl.hpp"

namespace xserialization::json
{
    namespace
    {
        auto &get(inner::OpaqueJSON *opaque_json, const Context &context)
        {
            auto &json = inner::toJSONImpl(opaque_json);
            try
            {
                switch(context.getType())
                {
                    case Context::TYPE_NAME:
                        return json[context.getName()];
                    case Context::TYPE_INDEX:
                        return json[context.getIndex()];
                    case Context::TYPE_NONE:
                        return json;
                    default:
                        assert(false);
                        return json;
                }
            }
            catch(const inner::JSONImpl::exception &exc)
            {
                throw exception::SerializerException(context, "invalid context");
            }
        }

        template<typename T>
        void writeValue(inner::OpaqueJSON *json, const T &value,
                const Context &context)
        {
            get(json, context) = value;
        }
    }

    void JSONSerializer::write(const IDeserializer &value, const Context &context)
    {
        auto &cur = get(json, context);
        switch(value.contextType())
        {
            case Context::TYPE_NAME:
                cur = inner::JSONImpl(inner::JSONImpl::value_t::object);
                break;
            case Context::TYPE_INDEX:
                cur = inner::JSONImpl(inner::JSONImpl::value_t::array);
                break;
            case Context::TYPE_NONE:
                cur.clear();
                break;
            default:
                assert(false);
                break;
        }
        JSONSerializer serializer(inner::toJSONOpaque(cur));
        value.visit(serializer);
    }

    void JSONSerializer::write(Null, const Context &context)
    {
        get(json, context).clear();
    }

    void JSONSerializer::write(bool value, const Context &context)
    {
        writeValue(json, value, context);
    }
    void JSONSerializer::write(char value, const Context &context)
    {
        writeValue(json, value, context);
    }
    void JSONSerializer::write(signed char value, const Context &context)
    {
        writeValue(json, value, context);
    }
    void JSONSerializer::write(unsigned char value, const Context &context)
    {
        writeValue(json, value, context);
    }
    void JSONSerializer::write(short value, const Context &context)
    {
        writeValue(json, value, context);
    }
    void JSONSerializer::write(unsigned short value, const Context &context)
    {
        writeValue(json, value, context);
    }
    void JSONSerializer::write(int value, const Context &context)
    {
        writeValue(json, value, context);
    }
    void JSONSerializer::write(unsigned int value, const Context &context)
    {
        writeValue(json, value, context);
    }
    void JSONSerializer::write(long value, const Context &context)
    {
        writeValue(json, value, context);
    }
    void JSONSerializer::write(unsigned long value, const Context &context)
    {
        writeValue(json, value, context);
    }
    void JSONSerializer::write(long long value, const Context &context)
    {
        writeValue(json, value, context);
    }
    void JSONSerializer::write(unsigned long long value, const Context &context)
    {
        writeValue(json, value, context);
    }
    void JSONSerializer::write(float value, const Context &context)
    {
        writeValue(json, value, context);
    }
    void JSONSerializer::write(double value, const Context &context)
    {
        writeValue(json, value, context);
    }
    void JSONSerializer::write(long double value, const Context &context)
    {
        writeValue(json, value, context);
    }
    void JSONSerializer::write(const std::string &value, const Context &context)
    {
        writeValue(json, value, context);
    }
}
