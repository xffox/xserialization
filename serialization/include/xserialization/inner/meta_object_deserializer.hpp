#ifndef XSERIALIZATION_INNER_METAOBJECTDESERIALIZER_HPP
#define XSERIALIZATION_INNER_METAOBJECTDESERIALIZER_HPP

#include "xserialization/context.hpp"
#include "xserialization/deserializer.hpp"
#include "xserialization/inner/meta_object.hpp"

namespace xserialization::inner
{
    class MetaObjectDeserializer: public IDeserializer
    {
    public:
        template<typename T>
        explicit MetaObjectDeserializer(const T &object)
            :object(object), fields_(object::MetaObjectTrait<T>::TYPE::fields())
        {}

        [[nodiscard]]
        Context::Type contextType() const override;

        void visit(ISerializer &serializer) const override;

    private:
        const object::MetaObject &object;
        const object::MetaObject::FieldMap &fields_;
    };
}

#endif
