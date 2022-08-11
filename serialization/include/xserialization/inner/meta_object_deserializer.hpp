#ifndef XSERIALIZATION_INNER_METAOBJECTDESERIALIZER_HPP
#define XSERIALIZATION_INNER_METAOBJECTDESERIALIZER_HPP

#include "xserialization/deserializer.hpp"
#include "xserialization/inner/meta_object.hpp"

namespace xserialization::inner
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
