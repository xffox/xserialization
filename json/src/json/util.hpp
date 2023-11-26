#ifndef XSERIALIZATION_JSON_UTIL_HPP
#define XSERIALIZATION_JSON_UTIL_HPP

#include <type_traits>
#include <utility>
#include <cassert>

namespace xserialization::json::util
{
    template<typename T,
        typename = std::enable_if_t<std::is_constructible_v<bool, T>>>
    constexpr decltype(auto) assertHasValue(T &&val)
    {
        assert(val);
        return std::forward<T>(val);
    }
}

#endif
