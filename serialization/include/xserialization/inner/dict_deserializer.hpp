#ifndef XSERIALIZATION_INNER_DICTDESERIALIZER_HPP
#define XSERIALIZATION_INNER_DICTDESERIALIZER_HPP

#include "xserialization/deserializer.hpp"
#include "xserialization/context.hpp"
#include "xserialization/inner/dict_trait.hpp"

namespace xserialization::inner
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

#include "xserialization/util.hpp"
#include "xserialization/exception/deserializer_exception.hpp"

namespace xserialization::inner
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
