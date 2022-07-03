#ifndef SERIALIZATION_DESERIALIZER_HPP
#define SERIALIZATION_DESERIALIZER_HPP

#include "serialization/context.hpp"

namespace serialization
{
    class ISerializer;

    class IDeserializer
    {
    public:
        virtual ~IDeserializer() = default;

        virtual Context::Type contextType() const = 0;

        virtual void visit(ISerializer &serializer) const = 0;
    };
}

#include "serialization/serializer.hpp"

#endif
