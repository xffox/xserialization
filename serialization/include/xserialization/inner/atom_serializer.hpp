#ifndef XSERIALIZATION_INNER_ATOMSERIALIZER_HPP
#define XSERIALIZATION_INNER_ATOMSERIALIZER_HPP

#include <string>

#include "xserialization/base_serializer.hpp"
#include "xserialization/context.hpp"
#include "xserialization/exception/serializer_exception.hpp"

namespace xserialization::inner
{
    namespace inner
    {
        template<typename V>
        struct ArgType
        {
            using Type = V;
        };
        template<>
        struct ArgType<std::string>
        {
            using Type = const std::string&;
        };
    }

    template<typename T>
    class AtomSerializer: public BaseSerializer
    {
        using TargetArgType = typename inner::ArgType<T>::Type;

    public:
        explicit AtomSerializer(T &value)
            :value(value)
        {}

        using BaseSerializer::write;

        [[nodiscard]]
        Context::Type contextType() const override
        {
            return Context::TYPE_NONE;
        }

        void write(TargetArgType value, const Context &context) override
        {
            if(context.getType() != Context::TYPE_NONE)
            {
                throw exception::SerializerException("invalid context");
            }
            this->value = value;
        }

    protected:
        bool prepareContext(const Context::Type contextType) override
        {
            return contextType == Context::TYPE_NONE;
        }

    private:
        T &value;
    };
}

#endif
