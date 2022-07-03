#ifndef SERIALIZATION_EXCEPTION_SERIALIZATIONEXCEPTION_HPP
#define SERIALIZATION_EXCEPTION_SERIALIZATIONEXCEPTION_HPP

#include <exception>
#include <type_traits>
#include <utility>

#include "serialization/context.hpp"

namespace serialization::exception
{
    class SerializationException: public std::exception
    {
        public:
            SerializationException()
                :msg_{}
            {}

            template<typename T, typename =
                std::enable_if_t<std::is_constructible_v<std::string, T>>>
            explicit SerializationException(T &&msg)
                :msg_(std::forward<T>(msg))
            {}

            ~SerializationException() throw() override
            {}

            const char *what() const throw() override
            {
                return "serialization exception";
            }

            const std::string &msg() const
            {
                return msg_;
            }

        private:
            std::string msg_;
    };
}

#endif
