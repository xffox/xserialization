#ifndef SERIALIZATION_SERIALIZATIONTRAIT_HPP
#define SERIALIZATION_SERIALIZATIONTRAIT_HPP

namespace serialization
{
    template<typename T, typename = void>
    struct SerializationTrait;

    template<typename T, typename = void>
    struct DeserializationTrait;
}

#include "serialization/inner/builtin_serialization_traits.hpp"

#endif
