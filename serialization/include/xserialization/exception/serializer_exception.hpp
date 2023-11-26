#ifndef XSERIALIZATION_EXCEPTION_SERIALIZEREXCEPTION_HPP
#define XSERIALIZATION_EXCEPTION_SERIALIZEREXCEPTION_HPP

#include <string>
#include <type_traits>
#include <utility>

#include "xserialization/context.hpp"
#include "xserialization/exception/serialization_exception.hpp"

namespace xserialization::exception
{
    class SerializerException: public SerializationException
    {
    public:
        explicit SerializerException(const Context &context)
            :context(context)
        {}

        template<typename T, typename =
            std::enable_if_t<std::is_constructible_v<std::string, T>>>
        SerializerException(const Context &context, T &&msg)
            :SerializationException(std::forward<T>(msg)), context(context)
        {}

        [[nodiscard]]
        const Context &getContext() const
        {
            return context;
        }

    private:
        Context context;
    };

    class TypeSerializerException: public SerializerException
    {
    public:
        explicit TypeSerializerException(const Context &context)
            :SerializerException(context)
        {}

        template<typename T, typename =
            std::enable_if_t<std::is_constructible_v<std::string, T>>>
        TypeSerializerException(const Context &context, T &&msg)
            :SerializerException(context, std::forward<T>(msg))
        {}
    };
}

#endif
