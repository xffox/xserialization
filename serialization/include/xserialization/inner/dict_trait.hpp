#ifndef XSERIALIZATION_INNER_DICTTRAIT_HPP
#define XSERIALIZATION_INNER_DICTTRAIT_HPP

#include <type_traits>
#include <utility>

namespace xserialization::inner
{
    template<typename Dict, typename = void>
    struct IsDict
    {
        static constexpr bool VALUE = false;
    };
    template<typename Dict>
    struct IsDict<Dict, std::void_t<
            typename Dict::mapped_type,
            typename Dict::key_type,
            typename Dict::value_type,
            typename Dict::iterator,
            std::enable_if_t<std::is_same_v<
                std::decay_t<decltype(std::declval<
                        typename Dict::value_type>().first)>,
                typename Dict::key_type>>,
            std::enable_if_t<std::is_same_v<
                std::decay_t<decltype(std::declval<
                        typename Dict::value_type>().second)>,
                typename Dict::mapped_type>>,
            std::enable_if_t<std::is_same_v<
                std::decay_t<decltype(*std::begin(std::declval<Dict>()))>,
                typename Dict::value_type>>,
            decltype(std::declval<Dict>().emplace(
                        std::make_pair(std::declval<typename Dict::key_type>(),
                            std::declval<typename Dict::mapped_type>()))),
            decltype(std::declval<Dict>().erase(
                        std::declval<typename Dict::iterator>()))
            >>
    {
        static constexpr bool VALUE = true;
    };

    template<typename Dict>
    inline constexpr auto IsDictType = IsDict<Dict>::VALUE;
}

#endif
