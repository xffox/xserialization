#ifndef XSERIALIZATION_EXCEPTION_DESERIALIZEREXCEPTION_HPP
#define XSERIALIZATION_EXCEPTION_DESERIALIZEREXCEPTION_HPP

#include <string>
#include <type_traits>
#include <utility>

#include "xserialization/exception/serialization_exception.hpp"

namespace xserialization::exception
{
    class DeserializerException: public SerializationException
    {
    public:
        DeserializerException()
            :SerializationException()
        {}

        template<typename T, typename =
            std::enable_if_t<std::is_constructible_v<std::string, T>>>
        DeserializerException(T &&msg)
            :SerializationException(std::forward<T>(msg))
        {}
    };
}

#endif
