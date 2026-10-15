#include <type_traits>

#include "xserialization/typeutil.hpp"

using namespace xserialization::typeutil;

static_assert(std::is_same_v<
    typename TypeList<int,
    double, short>::template Filter<std::is_integral>,
    TypeList<int, short>>);
static_assert(std::is_same_v<
    typename TypeList<bool, int>::template Filter<std::is_integral>,
    TypeList<bool, int>>);
static_assert(std::is_same_v<
    typename TypeList<bool, int, float>::template Filter<std::is_enum>,
    TypeList<>>);
