#include "serialization/inner/meta_object.hpp"

namespace serialization::inner
{
    MetaObject::~MetaObject() = default;

    Context::Type MetaObject::contextType() const
    {
        return Context::TYPE_NAME;
    }

    IField::~IField() = default;
}
