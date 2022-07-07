#ifndef SERIALIZATION_INNER_DICTDESERIALIZER_HPP
#define SERIALIZATION_INNER_DICTDESERIALIZER_HPP

#include "serialization/deserializer.hpp"
#include "serialization/context.hpp"
#include "serialization/inner/dict_trait.hpp"

namespace serialization::inner
{
    template<typename Dict>
    class DictDeserializer: public IDeserializer
    {
        static_assert(IsDictType<Dict>);
    public:
        DictDeserializer(const Dict &dict)
            :dict(dict)
        {}

        [[nodiscard]]
        Context::Type contextType() const override
        {
            return Context::TYPE_NAME;
        }

        void visit(ISerializer &serializer) const override;

    private:
        const Dict &dict;
    };
}

#include "serialization/util.hpp"
#include "serialization/exception/deserializer_exception.hpp"

namespace serialization::inner
{
    template<typename Dict>
    void DictDeserializer<Dict>::visit(ISerializer &serializer) const
    {
        if(serializer.contextType() == Context::TYPE_NAME)
        {
            for(const auto &p : dict)
            {
                util::visitValue(serializer, p.second, Context(p.first));
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
