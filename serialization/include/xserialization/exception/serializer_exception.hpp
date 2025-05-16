#ifndef XSERIALIZATION_EXCEPTION_SERIALIZEREXCEPTION_HPP
#define XSERIALIZATION_EXCEPTION_SERIALIZEREXCEPTION_HPP

#include <string>
#include <string_view>

#include "xserialization/context.hpp"
#include "xserialization/exception/serialization_exception.hpp"

namespace xserialization::exception
{
    class SerializerException: public SerializationException
    {
    public:
        explicit SerializerException(const Context &context)
            :SerializationException(prepareContextedMessage(context, {}))
        {}

        SerializerException(const Context &context, std::string_view msg)
            :SerializationException(prepareContextedMessage(context, msg)),
            context(context)
        {}

        [[nodiscard]]
        const Context &getContext() const
        {
            return context;
        }

    private:
        static std::string prepareContextedMessage(const Context &context,
                std::string_view msg);

        Context context;
    };

    class TypeSerializerException: public SerializerException
    {
    public:
        explicit TypeSerializerException(const Context &context)
            :SerializerException(context)
        {}

        TypeSerializerException(const Context &context, std::string_view msg)
            :SerializerException(context, msg)
        {}
    };
}

#endif
