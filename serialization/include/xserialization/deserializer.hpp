#ifndef XSERIALIZATION_DESERIALIZER_HPP
#define XSERIALIZATION_DESERIALIZER_HPP

#include "xserialization/context.hpp"

namespace xserialization
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

#include "xserialization/serializer.hpp"

#endif
