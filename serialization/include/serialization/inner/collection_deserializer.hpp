#ifndef SERIALIZATION_INNER_COLLECTIONDESERIALIZER_HPP
#define SERIALIZATION_INNER_COLLECTIONDESERIALIZER_HPP

#include "serialization/deserializer.hpp"
#include "serialization/context.hpp"
#include "serialization/inner/collection_trait.hpp"

namespace serialization::inner
{
    template<typename Collection>
    class CollectionDeserializer: public IDeserializer
    {
        static_assert(IsCollectionType<Collection>);
    public:
        explicit CollectionDeserializer(const Collection &collection)
            :collection(collection)
        {}

        [[nodiscard]]
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

#include "serialization/util.hpp"
#include "serialization/exception/deserializer_exception.hpp"

namespace serialization::inner
{
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
