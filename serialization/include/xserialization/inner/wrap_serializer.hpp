#ifndef XSERIALIZATION_INNER_WRAPSERIALIZER_HPP
#define XSERIALIZATION_INNER_WRAPSERIALIZER_HPP

#include "xserialization/context.hpp"
#include "xserialization/serializer.hpp"

namespace xserialization
{
    class IDeserializer;
}

namespace xserialization::inner
{
    class WrapSerializer: public ISerializer
    {
    public:
        explicit WrapSerializer(ISerializer &serializer)
            :serializer(serializer)
        {}

        [[nodiscard]]
        Context::Type contextType() const override;

        void write(const IDeserializer &value, const Context &context) override;
        void write(Null value, const Context &context) override;
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
        void write(const std::string& value, const Context &context) override;

    protected:
        // avoid clone interface with memory allocation
        virtual void visitWrapped(ISerializer &serializer,
            const IDeserializer &deserializer) const = 0;

    private:
        class WrapDeserializer;

        template<typename T>
        void writeValue(const T &value, const Context &context);

        ISerializer &serializer;
    };
}

#endif
