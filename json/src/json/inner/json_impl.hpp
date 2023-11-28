#ifndef XSERIALIZATION_JSON_JSONIMPL_HPP
#define XSERIALIZATION_JSON_JSONIMPL_HPP

#include <type_traits>

#include <nlohmann/json.hpp>

#include "xserialization/json/inner/opaque_json.hpp"

namespace xserialization::json::inner
{
    using JSONImpl = nlohmann::json;

    template<typename OpaqueType,
        typename = std::enable_if_t<std::is_same_v<
            std::remove_const_t<OpaqueType>, OpaqueJSON>>>
    constexpr auto &toJSONImpl(OpaqueType *const json)
    {
        return *static_cast<
            std::conditional_t<std::is_const_v<OpaqueType>,
                const JSONImpl, JSONImpl>*>(json);
    }

    template<typename ImplType,
        typename = std::enable_if_t<std::is_same_v<
            std::remove_const_t<ImplType>, JSONImpl>>>
    constexpr auto *toJSONOpaque(ImplType &json)
    {
        return &json;
    }
}

#endif
