#include "xserialization/inner/wrap_serializer.hpp"

#include "xserialization/deserializer.hpp"

namespace xserialization::inner
{
    class WrapSerializer::WrapDeserializer: public IDeserializer
    {
    public:
        explicit WrapDeserializer(const IDeserializer &deserializer,
            const WrapSerializer &wrapSerializer)
            :deserializer(deserializer), wrapSerializer(wrapSerializer)
        {}

        [[nodiscard]]
        Context::Type contextType() const override
        {
            return deserializer.contextType();
        }

        void visit(ISerializer &serializer) const override
        {
            wrapSerializer.visitWrapped(serializer, deserializer);
        }

    private:
        const IDeserializer &deserializer;
        const WrapSerializer &wrapSerializer;
    };

    Context::Type WrapSerializer::contextType() const
    {
        return serializer.contextType();
    }

    void WrapSerializer::write(const IDeserializer &value, const Context &context)
    {
        writeValue(WrapDeserializer(value, *this), context);
    }
    void WrapSerializer::write(Null value, const Context &context)
    {
        writeValue(value, context);
    }
    void WrapSerializer::write(bool value, const Context &context)
    {
        writeValue(value, context);
    }
    void WrapSerializer::write(char value, const Context &context)
    {
        writeValue(value, context);
    }
    void WrapSerializer::write(signed char value, const Context &context)
    {
        writeValue(value, context);
    }
    void WrapSerializer::write(unsigned char value, const Context &context)
    {
        writeValue(value, context);
    }
    void WrapSerializer::write(short value, const Context &context)
    {
        writeValue(value, context);
    }
    void WrapSerializer::write(unsigned short value, const Context &context)
    {
        writeValue(value, context);
    }
    void WrapSerializer::write(int value, const Context &context)
    {
        writeValue(value, context);
    }
    void WrapSerializer::write(unsigned int value, const Context &context)
    {
        writeValue(value, context);
    }
    void WrapSerializer::write(long value, const Context &context)
    {
        writeValue(value, context);
    }
    void WrapSerializer::write(unsigned long value, const Context &context)
    {
        writeValue(value, context);
    }
    void WrapSerializer::write(long long value, const Context &context)
    {
        writeValue(value, context);
    }
    void WrapSerializer::write(unsigned long long value, const Context &context)
    {
        writeValue(value, context);
    }
    void WrapSerializer::write(float value, const Context &context)
    {
        writeValue(value, context);
    }
    void WrapSerializer::write(double value, const Context &context)
    {
        writeValue(value, context);
    }
    void WrapSerializer::write(long double value, const Context &context)
    {
        writeValue(value, context);
    }
    void WrapSerializer::write(const std::string& value, const Context &context)
    {
        writeValue(value, context);
    }

    template<typename T>
    void WrapSerializer::writeValue(const T &value, const Context &context)
    {
        serializer.write(value, context);
    }
}
