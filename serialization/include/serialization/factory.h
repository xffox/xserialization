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
        std::auto_ptr<ISerializer> createSerializer(MetaObject &value);
        inline std::auto_ptr<const ISerializer> createSerializer(
            const MetaObject &value)
        {
            return std::auto_ptr<const ISerializer>(
                createSerializer(const_cast<MetaObject&>(value)).release());
        }

        template<typename T>
        std::auto_ptr<ISerializer> createSerializer(
            std::vector<T> &value);
        template<typename T>
        inline std::auto_ptr<const ISerializer> createSerializer(
            const std::vector<T> &value)
        {
            return std::auto_ptr<const ISerializer>(
                createSerializer(const_cast<std::vector<T>&>(
                        value)).release());
        }
    }
}

#include "serialization/CollectionSerializer.h"

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
    }
}

#endif
