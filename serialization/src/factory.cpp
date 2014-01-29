#include "serialization/factory.h"

#include "serialization/MetaObjectSerializer.h"
#include "serialization/MetaObjectDeserializer.h"

namespace serialization
{
    namespace factory
    {
        std::auto_ptr<ISerializer> createSerializer(MetaObject &value)
        {
            return std::auto_ptr<ISerializer>(new MetaObjectSerializer(
                    value));
        }

        std::auto_ptr<IDeserializer> createDeserializer(
            const MetaObject &value)
        {
            return std::auto_ptr<IDeserializer>(new MetaObjectDeserializer(
                    value));
        }
    }
}
