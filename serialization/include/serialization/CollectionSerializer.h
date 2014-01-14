#ifndef SERIALIZATION_COLLECTIONSERIALIZER_H
#define SERIALIZATION_COLLECTIONSERIALIZER_H

#include "serialization/ISerializer.h"
#include "serialization/exception/SerializationException.h"
#include "serialization/serialization.h"

namespace serialization
{
    namespace inner
    {
        template<class Collection, typename T>
        void writeByIndex(Collection &collection, const T &value, const Context &context)
        {
            if(context.getType() == Context::TYPE_INDEX && context.getIndex() < collection.size())
                collection[context.getIndex()] = value;
            else
                throw exception::SerializationException(context);
        }

        class BaseCollectionSerializer: public ISerializer
        {
        public:
            void write(bool value, const Context &context)
            {
                writeValue(value, context);
            }
            void write(char value, const Context &context)
            {
                writeValue(value, context);
            }
            void write(signed char value, const Context &context)
            {
                writeValue(value, context);
            }
            void write(unsigned char value, const Context &context)
            {
                writeValue(value, context);
            }
            void write(short value, const Context &context)
            {
                writeValue(value, context);
            }
            void write(unsigned short value, const Context &context)
            {
                writeValue(value, context);
            }
            void write(int value, const Context &context)
            {
                writeValue(value, context);
            }
            void write(unsigned int value, const Context &context)
            {
                writeValue(value, context);
            }
            void write(long value, const Context &context)
            {
                writeValue(value, context);
            }
            void write(unsigned long value, const Context &context)
            {
                writeValue(value, context);
            }
            void write(long long value, const Context &context)
            {
                writeValue(value, context);
            }
            void write(unsigned long long value, const Context &context)
            {
                writeValue(value, context);
            }
            void write(float value, const Context &context)
            {
                writeValue(value, context);
            }
            void write(double value, const Context &context)
            {
                writeValue(value, context);
            }
            void write(long double value, const Context &context)
            {
                writeValue(value, context);
            }
            void write(const std::string &value, const Context &context)
            {
                writeValue(value, context);
            }

        protected:
            // need writeValue because of std::string reference type parameter
            virtual void writeValue(const bool&, const Context &context)
            {
                throw exception::SerializationException(context);
            }
            virtual void writeValue(const char&, const Context &context)
            {
                throw exception::SerializationException(context);
            }
            virtual void writeValue(const signed char&, const Context &context)
            {
                throw exception::SerializationException(context);
            }
            virtual void writeValue(const unsigned char&, const Context &context)
            {
                throw exception::SerializationException(context);
            }
            virtual void writeValue(const short&, const Context &context)
            {
                throw exception::SerializationException(context);
            }
            virtual void writeValue(const unsigned short&, const Context &context)
            {
                throw exception::SerializationException(context);
            }
            virtual void writeValue(const int&, const Context &context)
            {
                throw exception::SerializationException(context);
            }
            virtual void writeValue(const unsigned int&, const Context &context)
            {
                throw exception::SerializationException(context);
            }
            virtual void writeValue(const long&, const Context &context)
            {
                throw exception::SerializationException(context);
            }
            virtual void writeValue(const unsigned long&, const Context &context)
            {
                throw exception::SerializationException(context);
            }
            virtual void writeValue(const long long&, const Context &context)
            {
                throw exception::SerializationException(context);
            }
            virtual void writeValue(const unsigned long long&, const Context &context)
            {
                throw exception::SerializationException(context);
            }
            virtual void writeValue(const float&, const Context &context)
            {
                throw exception::SerializationException(context);
            }
            virtual void writeValue(const double&, const Context &context)
            {
                throw exception::SerializationException(context);
            }
            virtual void writeValue(const long double&, const Context &context)
            {
                throw exception::SerializationException(context);
            }
            virtual void writeValue(const std::string&, const Context &context)
            {
                throw exception::SerializationException(context);
            }
        };
    }

    template<class Collection>
    class CollectionSerializer: public inner::BaseCollectionSerializer
    {
    public:
        CollectionSerializer(Collection &collection)
        :collection(collection)
        {
        }

        virtual std::auto_ptr<ISerializer> beginNamedCollection(const Context &context);
        virtual std::auto_ptr<ISerializer> beginIndexedCollection(const Context &context);

        virtual Context::Type contextType() const
        {
            return Context::TYPE_INDEX;
        }

        virtual void writeNull(const Context &context)
        {
            throw exception::SerializationException(context);
        }

        virtual void visit(ISerializer &serializer,
            const serialization::Context &context) const;

    protected:
        virtual void writeValue(const typename Collection::value_type &value,
            const Context &context);

    private:
        Collection &collection;
    };

    template<class Collection>
    std::auto_ptr<ISerializer> CollectionSerializer<Collection>::beginNamedCollection(const Context &context)
    {
        if(context.getType() == Context::TYPE_INDEX && context.getIndex() < collection.size())
        {
            std::auto_ptr<ISerializer> serializer =
                beginCollection(collection[context.getIndex()]);
            if(serializer.get() && serializer->contextType() == Context::TYPE_NAME)
                return serializer;
        }
        throw exception::SerializationException(context);
    }

    template<class Collection>
    std::auto_ptr<ISerializer> CollectionSerializer<Collection>::beginIndexedCollection(const Context &context)
    {
        if(context.getType() == Context::TYPE_INDEX && context.getIndex() < collection.size())
        {
            std::auto_ptr<ISerializer> serializer =
                beginCollection(collection[context.getIndex()]);
            if(serializer.get() && serializer->contextType() == Context::TYPE_INDEX)
                return serializer;
        }
        throw exception::SerializationException(context);
    }

    template<class Collection>
    void CollectionSerializer<Collection>::writeValue(
        const typename Collection::value_type &value, const Context &context)
    {
        if(context.getType() == Context::TYPE_INDEX &&
            context.getIndex() < collection.size())
            collection[context.getIndex()] = value;
        else
            throw exception::SerializationException(context);
    }

    template<class Collection>
    void CollectionSerializer<Collection>::visit(ISerializer &serializer, const serialization::Context &context) const
    {
        std::auto_ptr<serialization::ISerializer> s =
            serializer.beginIndexedCollection(context);
        assert(s.get());
        for(std::size_t i = 0; i < collection.size(); ++i)
            serialization::write(*s, collection[i], i);
    }
}

#endif