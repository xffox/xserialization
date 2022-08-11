#include "xserialization/inner/meta_object_deserializer.hpp"

#include <iterator>

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
        const auto &fields = object.fields();
        for(auto iter = std::begin(fields), endIter = std::end(fields);
                iter != endIter; ++iter)
        {
            iter->second->visit(serializer, object);
        }
    }
}
