#include "xserialization/inner/meta_object_deserializer.hpp"

#include "xserialization/context.hpp"
#include "xserialization/inner/meta_field.hpp"
#include "xserialization/exception/deserializer_exception.hpp"

namespace xserialization::inner
{
    Context::Type MetaObjectDeserializer::contextType() const
    {
        return Context::TYPE_NAME;
    }

    void MetaObjectDeserializer::visit(ISerializer &serializer) const
    {
        if(serializer.contextType() != Context::TYPE_NAME)
        {
            throw exception::DeserializerException("invalid context type");
        }
        for(const auto &[name, field] : fields_)
        {
            field->makeDeserializer(&object)->visit(serializer);
        }
    }
}
