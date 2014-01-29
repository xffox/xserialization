#ifndef SERIALIZATION_COLLECTIONDESERIALIZER_H
#define SERIALIZATION_COLLECTIONDESERIALIZER_H

#include "serialization/IDeserializer.h"
#include "serialization/ISerializer.h"
#include "serialization/Context.h"
#include "serialization/serialization.h"
#include "serialization/exception/SerializationException.h"

namespace serialization
{
    template<typename Collection>
    class CollectionDeserializer: public IDeserializer
    {
    public:
        CollectionDeserializer(const Collection &collection)
            :collection(collection)
        {}

        virtual void visit(ISerializer &serializer,
            const serialization::Context &context) const;

    private:
        const Collection &collection;
    };

    template<class Collection>
    void CollectionDeserializer<Collection>::visit(ISerializer &serializer,
        const serialization::Context &context) const
    {
        std::auto_ptr<serialization::ISerializer> s =
            serializer.beginCollection(Context::TYPE_INDEX, context);
        if(s.get())
            for(std::size_t i = 0; i < collection.size(); ++i)
                serialization::write(*s, collection[i], i);
        else
            throw exception::SerializationException(context);
    }
}

#endif
