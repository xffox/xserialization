#include "xserialization/inner/meta_object_serializer.hpp"

#include <algorithm>
#include <iterator>

#include "xserialization/inner/attribute.hpp"

namespace xserialization::inner
{
    bool MetaObjectSerializer::allFieldsUsed() const
    {
        return unusedFields.empty();
    }

    bool MetaObjectSerializer::isOpenObject() const
    {
        return static_cast<bool>(object.attributes() &
                static_cast<AttrMask>(ClassAttribute::OPEN));
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
        for(const auto &field : fields)
        {
            if(!static_cast<bool>(field.second->attributes() &
                        static_cast<AttrMask>(FieldAttribute::OPTIONAL)))
            {
                result.insert(field.first);
            }
        }
        return result;
    }
}
