#ifndef SERIALIZATION_INNER_METAOBJECTDESERIALIZER_HPP
#define SERIALIZATION_INNER_METAOBJECTDESERIALIZER_HPP

#include "serialization/deserializer.hpp"
#include "serialization/inner/meta_object.hpp"

namespace serialization::inner
{
    class MetaObjectDeserializer: public IDeserializer
    {
    public:
        explicit MetaObjectDeserializer(const MetaObject &object)
            :object(object)
        {}

        Context::Type contextType() const override;

        void visit(ISerializer &serializer) const override;
        
    private:
        const MetaObject &object;
    };
}

#endif
