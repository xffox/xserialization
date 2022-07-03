#ifndef SERIALIZATION_EXCEPTION_DESERIALIZEREXCEPTION_HPP
#define SERIALIZATION_EXCEPTION_DESERIALIZEREXCEPTION_HPP

#include <string>
#include <type_traits>
#include <utility>

#include "serialization/exception/serialization_exception.hpp"

namespace serialization::exception
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
