#include "serialization//inner/meta_object_serializer.hpp"

#include <algorithm>
#include <iterator>

namespace serialization::inner
{
    bool MetaObjectSerializer::allFieldsUsed() const
    {
        return unusedFields.empty();
    }

    Context::Type MetaObjectSerializer::contextType() const
    {
        return Context::TYPE_NAME;
    }

    std::unordered_set<std::string> MetaObjectSerializer::prepareUnusedFields(
            const MetaObject &object)
    {
        std::unordered_set<std::string> result;
        const auto& fields = object.fields();
        std::transform(std::begin(fields), std::end(fields),
                std::inserter(result, std::end(result)),
                [](const auto &p){
                    return p.first;
                });
        return result;
    }
}
