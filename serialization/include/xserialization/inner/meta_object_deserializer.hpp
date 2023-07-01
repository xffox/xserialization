#ifndef XSERIALIZATION_INNER_METAOBJECTDESERIALIZER_HPP
#define XSERIALIZATION_INNER_METAOBJECTDESERIALIZER_HPP

#include <iterator>

#include "xserialization/deserializer.hpp"
#include "xserialization/inner/meta_object.hpp"
#include "xserialization/exception/deserializer_exception.hpp"

namespace xserialization::inner
{
    template<typename T>
    class MetaObjectDeserializer: public IDeserializer
    {
    public:
        explicit MetaObjectDeserializer(const T &object)
            :object(object)
        {}

        Context::Type contextType() const override
        {
            return Context::TYPE_NAME;
        }

        void visit(ISerializer &serializer) const override
        {
            if(serializer.contextType() != Context::TYPE_NAME)
            {
                throw exception::DeserializerException("invalid context type");
            }
            const auto &fields = object.fields();
            for(auto iter = std::begin(fields), endIter = std::end(fields);
                    iter != endIter; ++iter)
            {
                iter->second->visit(serializer, object);
            }
        }

    private:
        const T &object;
    };
}

#endif
