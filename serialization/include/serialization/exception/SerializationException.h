#ifndef SERIALIZATION_EXCEPTION_SERIALIZATIONEXCEPTION_H
#define SERIALIZATION_EXCEPTION_SERIALIZATIONEXCEPTION_H

#include <exception>

#include "serialization/Context.h"

namespace serialization
{
    namespace exception
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
}

#endif
