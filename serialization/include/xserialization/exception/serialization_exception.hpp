#ifndef XSERIALIZATION_EXCEPTION_SERIALIZATIONEXCEPTION_HPP
#define XSERIALIZATION_EXCEPTION_SERIALIZATIONEXCEPTION_HPP

#include <exception>
#include <type_traits>
#include <utility>

#include "xserialization/context.hpp"

namespace xserialization::exception
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

            [[nodiscard]]
            const char *what() const noexcept override
            {
                return "xserialization exception";
            }

            [[nodiscard]]
            const std::string &msg() const
            {
                return msg_;
            }

        private:
            std::string msg_;
    };
}

#endif
