#ifndef SERIALIZATION_IDESERIALIZER_H
#define SERIALIZATION_IDESERIALIZER_H

namespace serialization
{
    class ISerializer;
    class Context;

    class IDeserializer
    {
    public:
        virtual ~IDeserializer(){}
        
        virtual void visit(ISerializer &serializer,
            const serialization::Context &context) const = 0;
    };
}

#endif
