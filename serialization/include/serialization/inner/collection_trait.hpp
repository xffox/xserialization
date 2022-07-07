#ifndef SERIALIZATION_INNER_COLLECTIONTRAIT_HPP
#define SERIALIZATION_INNER_COLLECTIONTRAIT_HPP

#include <type_traits>
#include <utility>

namespace serialization::inner
{
    template<typename Collection, typename = void>
    struct IsCollection
    {
        static constexpr bool VALUE = false;
    };
    template<typename Collection>
    struct IsCollection<Collection, std::void_t<
            typename Collection::value_type,
            typename Collection::reference,
            typename Collection::size_type,
            typename Collection::iterator,
            std::enable_if<std::is_same_v<
                decltype(std::declval<Collection>()[
                        std::declval<typename Collection::size_type>()]),
                typename Collection::reference>>,
            std::enable_if_t<std::is_same_v<
                std::decay_t<decltype(*std::begin(std::declval<Collection>()))>,
                    typename Collection::value_type>>,
            decltype(++std::declval<typename Collection::iterator>()),
            decltype(std::declval<Collection>().resize(
                        std::declval<typename Collection::size_type>()))
            >>
    {
        static constexpr bool VALUE = true;
    };

    template<typename Collection>
    inline constexpr auto IsCollectionType = IsCollection<Collection>::VALUE;
}

#endif
