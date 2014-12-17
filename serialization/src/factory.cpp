#include "serialization/factory.h"

#include "serialization/MetaObjectSerializer.h"

namespace serialization
{
    namespace factory
    {
        std::unique_ptr<ISerializer> createSerializer(MetaObject &value)
        {
            return std::unique_ptr<ISerializer>(new MetaObjectSerializer(
                    value));
        }
    }
}
