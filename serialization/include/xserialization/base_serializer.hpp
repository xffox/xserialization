#ifndef XSERIALIZATION_BASESERIALIZER_HPP
#define XSERIALIZATION_BASESERIALIZER_HPP

#include "xserialization/context.hpp"
#include "xserialization/serializer.hpp"
#include "xserialization/deserializer.hpp"
#include "xserialization/inner/atom_deserializer.hpp"
#include "xserialization/exception/serializer_exception.hpp"

namespace xserialization
{
    class BaseSerializer: public ISerializer
    {
    public:
        ~BaseSerializer() override = 0;

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
        virtual bool prepareContext(Context::Type);

    private:
        template<typename T>
        void writeAtom(const T &value, const Context &context);

        static exception::TypeSerializerException prepareInvalidValueException(
                const Context &context);

        bool recurse = false;
    };
}

#endif
