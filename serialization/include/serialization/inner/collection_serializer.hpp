#ifndef SERIALIZATION_INNER_COLLECTIONSERIALIZER_HPP
#define SERIALIZATION_INNER_COLLECTIONSERIALIZER_HPP

#include <tuple>

#include "serialization/deserializer.hpp"
#include "serialization/serializer.hpp"
#include "serialization/context.hpp"
#include "serialization/base_serializer.hpp"
#include "serialization/typeutil.hpp"
#include "serialization/exception/serializer_exception.hpp"
#include "serialization/exception/deserializer_exception.hpp"

namespace serialization::inner
{
    template<class Collection>
    class CollectionSerializer: public BaseSerializer
    {
    public:
        using BaseSerializer::write;

        explicit CollectionSerializer(Collection &collection)
            :collection(collection)
        {}

        Context::Type contextType() const override
        {
            return Context::TYPE_INDEX;
        }

        void write(
                typeutil::WriteType<typename Collection::value_type> value,
                const Context &context) override
        {
            CollectionSerializer::writeValue(value, context);
        }

        void prepareContext(Context::Type type) override
        {
            if(type != Context::TYPE_INDEX)
            {
                // TODO: better exception
                throw exception::SerializerException(Context());
            }
        }

    protected:
        template<typename T>
        void writeValue(const T &value, const Context &context);

    private:
        Collection &collection;
    };

    template<typename Collection>
    class CollectionDeserializer: public IDeserializer
    {
    public:
        explicit CollectionDeserializer(const Collection &collection)
            :collection(collection)
        {}

        Context::Type contextType() const override
        {
            return Context::TYPE_INDEX;
        }

        void visit(ISerializer &serializer) const override;

    private:
        const Collection &collection;
    };
}

#include <iterator>
#include <type_traits>

#include "serialization/util.hpp"

namespace serialization::inner
{
    template<typename Collection>
    template<typename T>
    void CollectionSerializer<Collection>::writeValue(const T &value,
            const Context &context)
    {
        if(context.getType() == Context::TYPE_NONE)
        {
            if constexpr(std::is_same_v<std::decay_t<decltype(value)>, IDeserializer>)
            {
                BaseSerializer::write(value, context);
            }
            else
            {
                throw exception::SerializerException(context, "invalid value");
            }
        }
        else if(context.getType() == Context::TYPE_INDEX)
        {
            if(context.getIndex() >= collection.size())
                collection.resize(context.getIndex() + 1); // TODO: optimize
            if(!util::writeValue(collection[context.getIndex()], value))
            {
                throw exception::SerializerException(context, "invalid value");
            }
        }
        else
        {
            throw exception::SerializerException(context, "invalid context type");
        }
    }

    template<typename Collection>
    void CollectionDeserializer<Collection>::visit(ISerializer &serializer) const
    {
        if(serializer.contextType() == Context::TYPE_INDEX)
        {
            std::size_t i = 0;
            for(auto iter = std::begin(collection); iter != std::end(collection);
                    ++iter, ++i)
            {
                util::visitValue(serializer, *iter, Context(i));
            }
        }
        else if(serializer.contextType() == Context::TYPE_NONE)
        {
            serializer.write(*this, Context());
        }
        else
        {
            throw exception::DeserializerException("invalid value context type");
        }
    }
}

#endif
