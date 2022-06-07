#ifndef SERIALIZATION_EXCEPTION_SERIALIZATIONEXCEPTION_HPP
#define SERIALIZATION_EXCEPTION_SERIALIZATIONEXCEPTION_HPP

#include <exception>

#include "serialization/context.hpp"

namespace serialization::exception
{
    class SerializationException: public std::exception
    {
        public:
            SerializationException(const Context &context)
                :context(context)
            {}

            virtual ~SerializationException() throw()
            {}

            virtual const char *what() const throw()
            {
                return "serialization exception";
            }

            const Context &getContext() const
            {
                return context;
            }

        private:
            Context context;
    };
}

#endif
