#ifndef SERIALIZATION_FACTORY_H
#define SERIALIZATION_FACTORY_H

#include <memory>
#include <vector>

#include "serialization/ISerializer.h"
#include "serialization/IDeserializer.h"

namespace serialization
{
    class MetaObject;

    namespace factory
    {
        std::auto_ptr<ISerializer> createSerializer(MetaObject &value);
        template<typename T>
        std::auto_ptr<ISerializer> createSerializer(
            std::vector<T> &value);

        std::auto_ptr<IDeserializer> createDeserializer(
            const MetaObject &value);
        template<typename T>
        std::auto_ptr<IDeserializer> createDeserializer(
            const std::vector<T> &value);
    }
}

#include "serialization/CollectionSerializer.h"
#include "serialization/CollectionDeserializer.h"

namespace serialization
{
    namespace factory
    {
        template<typename T>
        std::auto_ptr<ISerializer> createSerializer(std::vector<T> &value)
        {
            return std::auto_ptr<ISerializer>(
                new CollectionSerializer<std::vector<T> >(value));
        }

        template<typename T>
        std::auto_ptr<IDeserializer> createDeserializer(
            const std::vector<T> &value)
        {
            return std::auto_ptr<IDeserializer>(
                new CollectionDeserializer<std::vector<T> >(value));
        }
    }
}

#endif
