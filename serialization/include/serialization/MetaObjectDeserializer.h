#ifndef SERIALIZATION_METAOBJECTDESERIALIZER_H
#define SERIALIZATION_METAOBJECTDESERIALIZER_H

#include "serialization/IDeserializer.h"

namespace serialization
{
    class MetaObject;

    class MetaObjectDeserializer: public IDeserializer
    {
    public:
        MetaObjectDeserializer(const MetaObject &object)
            :object(object)
        {}

        virtual void visit(ISerializer &serializer,
            const serialization::Context &context) const;

    private:
        const MetaObject &object;
    };
}

#endif
