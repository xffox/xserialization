#ifndef XSERIALIZATION_INNER_ATOMDESERIALIZER_HPP
#define XSERIALIZATION_INNER_ATOMDESERIALIZER_HPP

#include "xserialization/deserializer.hpp"
#include "xserialization/serializer.hpp"
#include "xserialization/context.hpp"

namespace xserialization::inner
{
    template<typename T>
    class AtomDeserializer: public IDeserializer
    {
    public:
        explicit AtomDeserializer(const T &value)
            :value(value)
        {}

        [[nodiscard]]
        Context::Type contextType() const override
        {
            return Context::TYPE_NONE;
        }

        void visit(ISerializer &serializer) const override
        {
            serializer.write(value, Context());
        }

    private:
        const T &value;
    };
}

#endif
