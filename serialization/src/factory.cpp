#include "serialization/factory.h"

#include "serialization/MetaObjectSerializer.h"

namespace serialization
{
    namespace factory
    {
        std::auto_ptr<ISerializer> createSerializer(MetaObject &value)
        {
            return std::auto_ptr<ISerializer>(new MetaObjectSerializer(
                    value));
        }
    }
}
