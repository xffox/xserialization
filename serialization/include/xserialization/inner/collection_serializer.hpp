#ifndef XSERIALIZATION_INNER_COLLECTIONSERIALIZER_HPP
#define XSERIALIZATION_INNER_COLLECTIONSERIALIZER_HPP

#include "xserialization/base_serializer.hpp"
#include "xserialization/context.hpp"
#include "xserialization/typeutil.hpp"
#include "xserialization/inner/collection_trait.hpp"

namespace xserialization::inner
{
    template<class Collection>
    class CollectionSerializer: public BaseSerializer
    {
        static_assert(IsCollectionType<Collection>);
    public:
        using BaseSerializer::write;

        explicit CollectionSerializer(Collection &collection)
            :collection(collection)
        {}

        [[nodiscard]]
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

    protected:
        bool prepareContext(Context::Type type) override;

        template<typename T>
        void writeValue(const T &value, const Context &context);

    private:
        Collection &collection;
    };
}

#include <type_traits>
#include <iterator>

#include "xserialization/util.hpp"
#include "xserialization/exception/serializer_exception.hpp"

namespace xserialization::inner
{
    template<typename Collection>
    bool CollectionSerializer<Collection>::prepareContext(Context::Type type)
    {
        return (type == Context::TYPE_INDEX);
    }

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
            {
                collection.resize(context.getIndex() + 1);
            }
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
}

#endif
