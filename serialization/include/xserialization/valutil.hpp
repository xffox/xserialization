#ifndef XSERIALIZATION_VALUTIL_HPP
#define XSERIALIZATION_VALUTIL_HPP

#include <limits>
#include <type_traits>

namespace xserialization::valutil
{
    template<typename Target, typename Src,
        typename = std::enable_if_t<std::is_arithmetic_v<Src>>>
    constexpr bool canAssign(Src value)
    {
        if constexpr(std::is_signed_v<Target> == std::is_signed_v<Src> ||
                std::is_floating_point_v<Target>)
        {
            return (value >= std::numeric_limits<Target>::lowest() &&
                    value <= std::numeric_limits<Target>::max());
        }
        else if constexpr(std::is_unsigned_v<Target>)
        {
            return (value >= 0 &&
                    static_cast<std::make_unsigned_t<Src>>(value) <=
                    std::numeric_limits<Target>::max());
        }
        else
        {
            return (value <= static_cast<std::make_unsigned_t<Target>>(
                        std::numeric_limits<Target>::max()));
        }
    }
}

#endif
