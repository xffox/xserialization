#ifndef SERIALIZATION_EXCEPTION_SERIALIZEREXCEPTION_HPP
#define SERIALIZATION_EXCEPTION_SERIALIZEREXCEPTION_HPP

#include <string>
#include <type_traits>
#include <utility>

#include "serialization/context.hpp"
#include "serialization/exception/serialization_exception.hpp"

namespace serialization::exception
{
    class SerializerException: public SerializationException
    {
    public:
        SerializerException(const Context &context)
            :SerializationException(), context(context)
        {}

        template<typename T, typename =
            std::enable_if_t<std::is_constructible_v<std::string, T>>>
        SerializerException(const Context &context, T &&msg)
            :SerializationException(std::forward<T>(msg)), context(context)
        {}

        const Context &getContext() const
        {
            return context;
        }
        
    private:
        Context context;
    };
}

#endif
