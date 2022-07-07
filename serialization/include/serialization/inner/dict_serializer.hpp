#ifndef SERIALIZATION_INNER_DICTSERIALIZER_HPP
#define SERIALIZATION_INNER_DICTSERIALIZER_HPP

#include "serialization/base_serializer.hpp"
#include "serialization/context.hpp"
#include "serialization/typeutil.hpp"
#include "serialization/inner/dict_trait.hpp"

namespace serialization::inner
{
    template<typename Dict>
    class DictSerializer: public BaseSerializer
    {
        static_assert(IsDictType<Dict>);
    public:
        explicit DictSerializer(Dict &dict)
            :dict(dict)
        {}

        [[nodiscard]]
        Context::Type contextType() const override
        {
            return Context::TYPE_NAME;
        }

        void write(
                typeutil::WriteType<typename Dict::mapped_type> value,
                const Context &context) override
        {
            writeValue(value, context);
        }

        void prepareContext(Context::Type type) override;

    private:
        template<typename T>
        void writeValue(const T &value, const Context &context);

    private:
        Dict &dict;
    };
}

#include <type_traits>

#include "serialization/util.hpp"
#include "serialization/exception/serializer_exception.hpp"

namespace serialization::inner
{
    template<typename Dict>
    void DictSerializer<Dict>::prepareContext(Context::Type type)
    {
        if(type != Context::TYPE_NAME)
        {
            // TODO: better exception
            throw exception::SerializerException(Context());
        }
    }

    template<typename Dict>
    template<typename T>
    void DictSerializer<Dict>::writeValue(const T &value,
            const Context &context)
    {
        if(context.getType() == Context::TYPE_NONE)
        {
            if(std::is_same_v<std::decay_t<decltype(value)>, IDeserializer>)
            {
                BaseSerializer::write(value, context);
            }
            else
            {
                throw exception::SerializerException(context, "invalid value");
            }
        }
        else if(context.getType() == Context::TYPE_NAME)
        {
            auto iter = dict.emplace(context.getName(),
                    typename Dict::mapped_type{}).first;
            if(!util::writeValue(iter->second, value))
            {
                dict.erase(iter);
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
