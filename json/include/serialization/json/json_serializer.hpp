#ifndef SERIALIZATION_JSON_JSONSERIALIZER_HPP
#define SERIALIZATION_JSON_JSONSERIALIZER_HPP

#include <nlohmann/json.hpp>

#include <serialization/serializer.hpp>

namespace serialization::json
{
    class JSON;

    class JSONSerializer: public ISerializer
    {
        friend JSON;
    public:
        [[nodiscard]]
        Context::Type contextType() const override;

        void write(const IDeserializer &value, const Context &context) override;
        void write(Null, const Context &context) override;

        void write(bool value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(char value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(signed char value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(unsigned char value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(short value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(unsigned short value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(int value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(unsigned int value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(long value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(unsigned long value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(long long value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(unsigned long long value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(float value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(double value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(long double value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(const std::string &value, const Context &context) override
        {
            writeValue(value, context);
        }

    private:
        JSONSerializer(nlohmann::json &value)
            :value(value)
        {}

        nlohmann::json &get(const Context &context);

        template<typename T>
        void writeValue(const T &value, const Context &context)
        {
            get(context) = value;
        }

    private:
        nlohmann::json &value;
    };
}

#endif
