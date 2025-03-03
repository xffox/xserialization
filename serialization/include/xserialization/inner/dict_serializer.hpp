#ifndef XSERIALIZATION_INNER_DICTSERIALIZER_HPP
#define XSERIALIZATION_INNER_DICTSERIALIZER_HPP

#include "xserialization/base_serializer.hpp"
#include "xserialization/context.hpp"
#include "xserialization/typeutil.hpp"
#include "xserialization/inner/dict_trait.hpp"

namespace xserialization::inner
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

        using BaseSerializer::write;

        void write(
                typeutil::WriteType<typename Dict::mapped_type> value,
                const Context &context) override
        {
            writeValue(value, context);
        }

    protected:
        bool prepareContext(Context::Type type) override;

        template<typename T>
        void writeValue(const T &value, const Context &context);

    private:
        Dict &dict;
    };
}

#include <type_traits>

#include "xserialization/util.hpp"
#include "xserialization/exception/serializer_exception.hpp"

namespace xserialization::inner
{
    template<typename Dict>
    bool DictSerializer<Dict>::prepareContext(Context::Type type)
    {
        return (type == Context::TYPE_NAME);
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
