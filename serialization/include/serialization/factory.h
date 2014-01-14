#ifndef SERIALIZATION_FACTORY_H
#define SERIALIZATION_FACTORY_H

#include <memory>
#include <vector>

#include "serialization/ISerializer.h"

namespace serialization
{
    class MetaObject;

    namespace factory
    {
        std::auto_ptr<ISerializer> createSerializer(const MetaObject &value);
        template<typename T>
        std::auto_ptr<ISerializer> createSerializer(
            const std::vector<T> &value);
    }
}

#include "serialization/CollectionSerializer.h"

namespace serialization
{
    namespace factory
    {
        template<typename T>
        std::auto_ptr<ISerializer> createSerializer(
            const std::vector<T> &value)
        {
            return std::auto_ptr<ISerializer>(
                new CollectionSerializer<std::vector<T> >(
                    const_cast<std::vector<T>&>(value)));
        }
    }
}

#endif
