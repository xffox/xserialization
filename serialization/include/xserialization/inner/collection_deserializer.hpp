#ifndef XSERIALIZATION_INNER_COLLECTIONDESERIALIZER_HPP
#define XSERIALIZATION_INNER_COLLECTIONDESERIALIZER_HPP

#include "xserialization/deserializer.hpp"
#include "xserialization/context.hpp"
#include "xserialization/inner/collection_trait.hpp"

namespace xserialization::inner
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

#include "xserialization/util.hpp"
#include "xserialization/exception/deserializer_exception.hpp"

namespace xserialization::inner
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
