#ifndef XSERIALIZATION_UTIL_HPP
#define XSERIALIZATION_UTIL_HPP

#include <type_traits>
#include <tuple>

#include "xserialization/serializer.hpp"
#include "xserialization/context.hpp"
#include "xserialization/typeutil.hpp"

namespace xserialization::util
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
    void visitValue(xserialization::ISerializer &serializer, const R &value,
            const Context &context,
            std::enable_if_t<typeutil::IsSerializationTrivial<R>::value, int> = 0);
    template<typename R>
    void visitValue(xserialization::ISerializer &serializer, const R &value,
            const Context &context,
            std::enable_if_t<!typeutil::IsSerializationTrivial<R>::value, int> = 0);
}

#include "xserialization/operator.hpp"
#include "xserialization/serialization_trait.hpp"
#include "xserialization/exception/serialization_exception.hpp"

namespace xserialization::util
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
                xserialization::Context());
        return true;
    }

    template<typename R>
    void visitValue(xserialization::ISerializer &serializer, const R &value,
            const Context &context,
            std::enable_if_t<typeutil::IsSerializationTrivial<R>::value, int>)
    {
        serializer.write(value, context);
    }
    template<typename R>
    void visitValue(xserialization::ISerializer &serializer, const R &value,
            const Context &context,
            std::enable_if_t<!typeutil::IsSerializationTrivial<R>::value, int>)
    {
        serializer<<std::forward_as_tuple(value, context);
    }
}

#endif
