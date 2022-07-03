#ifndef SERIALIZATION_UTIL_HPP
#define SERIALIZATION_UTIL_HPP

#include <type_traits>
#include <tuple>

#include "serialization/serializer.hpp"
#include "serialization/context.hpp"
#include "serialization/typeutil.hpp"

namespace serialization::util
{
    template<typename R, typename T>
    bool writeValue(R &dst, const T &src,
            std::enable_if_t<typeutil::IsSerializationTrivial<R>::value &&
                typeutil::IsSerializationTrivial<T>::value, int> = 0);
    template<typename R, typename T>
    bool writeValue(R&, const T&,
            std::enable_if_t<typeutil::IsSerializationTrivial<R>::value &&
                !typeutil::IsSerializationTrivial<T>::value, int> = 0);
    template<typename R, typename T>
    bool writeValue(R &dst, const T &src,
            std::enable_if_t<!typeutil::IsSerializationTrivial<R>::value, int> = 0);

    template<typename R>
    void visitValue(serialization::ISerializer &serializer, const R &value,
            const Context &context,
            std::enable_if_t<typeutil::IsSerializationTrivial<R>::value, int> = 0);
    template<typename R>
    void visitValue(serialization::ISerializer &serializer, const R &value,
            const Context &context,
            std::enable_if_t<!typeutil::IsSerializationTrivial<R>::value, int> = 0);
}

#include "serialization/operator.hpp"
#include "serialization/serialization_trait.hpp"
#include "serialization/exception/serialization_exception.hpp"

namespace serialization::util
{
    template<typename R, typename T>
    bool writeValue(R &dst, const T &src,
            std::enable_if_t<typeutil::IsSerializationTrivial<R>::value &&
                typeutil::IsSerializationTrivial<T>::value, int>)
    {
        dst = src;
        return true;
    }
    template<typename R, typename T>
    bool writeValue(R&, const T&,
            std::enable_if_t<typeutil::IsSerializationTrivial<R>::value &&
                !typeutil::IsSerializationTrivial<T>::value, int>)
    {
        return false;
    }
    template<typename R, typename T>
    bool writeValue(R &dst, const T &src,
            std::enable_if_t<!typeutil::IsSerializationTrivial<R>::value, int>)
    {
        SerializationTrait<R>::toSerializer(dst).write(src,
                serialization::Context());
        return true;
    }

    template<typename R>
    void visitValue(serialization::ISerializer &serializer, const R &value,
            const Context &context,
            std::enable_if_t<typeutil::IsSerializationTrivial<R>::value, int>)
    {
        serializer.write(value, context);
    }
    template<typename R>
    void visitValue(serialization::ISerializer &serializer, const R &value,
            const Context &context,
            std::enable_if_t<!typeutil::IsSerializationTrivial<R>::value, int>)
    {
        serializer<<std::forward_as_tuple(value, context);
    }
}

#endif
