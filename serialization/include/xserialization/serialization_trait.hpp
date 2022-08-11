#ifndef XSERIALIZATION_SERIALIZATIONTRAIT_HPP
#define XSERIALIZATION_SERIALIZATIONTRAIT_HPP

namespace xserialization
{
    template<typename T, typename = void>
    struct SerializationTrait;

    template<typename T, typename = void>
    struct DeserializationTrait;
}

#include "xserialization/inner/builtin_serialization_traits.hpp"

#endif
