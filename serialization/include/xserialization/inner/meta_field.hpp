#ifndef XSERIALIZATION_INNER_METAFIELD_HPP
#define XSERIALIZATION_INNER_METAFIELD_HPP

#include <type_traits>
#include <string>
#include <utility>
#include <memory>

#include "xserialization/context.hpp"
#include "xserialization/serializer.hpp"
#include "xserialization/deserializer.hpp"
#include "xserialization/typeutil.hpp"
#include "xserialization/valutil.hpp"
#include "xserialization/exception/serializer_exception.hpp"
#include "xserialization/base_serializer.hpp"
#include "xserialization/inner/attribute.hpp"
#include "xserialization/util.hpp"

namespace xserialization::inner::field
{
    class IField
    {
    public:
        virtual ~IField() = default;

        [[nodiscard]]
        virtual AttrMask attributes() const = 0;

        // TODO: remove the allocation
        [[nodiscard]]
        virtual std::unique_ptr<ISerializer> makeSerializer(void*) const = 0;
        [[nodiscard]]
        virtual std::unique_ptr<IDeserializer> makeDeserializer(const void*) const = 0;
    };

    template<typename T>
    class FieldDeserializer: public IDeserializer
    {
    public:
        explicit FieldDeserializer(const T &src, std::string name)
            :src(src), name(std::move(name))
        {}

        [[nodiscard]]
        Context::Type contextType() const override
        {
            return Context::TYPE_NONE;
        }

        void visit(ISerializer &serializer) const override
        {
            xserialization::util::visitValue(serializer, src, Context(name));
        }

    protected:
        const T &src;
        // TODO: string_view
        std::string name;
    };

    namespace inner
    {
        template<typename T>
        inline constexpr bool IsWeakConvertible =
            (std::is_arithmetic_v<T> && !std::is_same_v<T, bool>);

        template<typename Target>
        struct ShouldConvert
        {
            template<typename Cand>
            struct Apply
            {
                static constexpr auto value = !std::is_same_v<Target, Cand> &&
                    inner::IsWeakConvertible<Target> && inner::IsWeakConvertible<Cand> &&
                    std::is_convertible_v<Cand, Target> &&
                    std::is_floating_point_v<Target> >= std::is_floating_point_v<Cand>;
            };
        };
    }

    template<typename Target, typename Cand>
    class BaseConvertedFieldSerializer: public virtual BaseSerializer
    {
    public:
        using BaseSerializer::write;

        void write(Cand value, const Context &context) override
        {
            if(context.getType() != Context::TYPE_NONE)
            {
                throw exception::SerializerException(context, "invalid context");
            }
            if(!valutil::canAssign<Target>(value))
            {
                throw exception::TypeSerializerException(context, "invalid field write");
            }
            return static_cast<ISerializer&>(*this).write(static_cast<Target>(value), context);
        }
    };

    template<typename Target, typename... Cands>
    class TargetConvertedFieldWithBases: public Cands...
    {
    public:
        explicit TargetConvertedFieldWithBases(Target &dst)
            :dst(dst)
        {}

        [[nodiscard]]
        Context::Type contextType() const override
        {
            return Context::TYPE_NONE;
        }

        using Cands::write...;

        void write(
                typeutil::WriteType<Target> value,
                const Context &context) override
        {
            if(!xserialization::util::writeValue(dst, value))
            {
                throw exception::TypeSerializerException(context, "invalid field write");
            }
        }

    protected:
        Target &dst;
    };

    template<typename Target, typename... Cands>
    struct TargetConvertedField
    {
        using Type = TargetConvertedFieldWithBases<Target, BaseConvertedFieldSerializer<Target, Cands>...>;
    };
    template<typename Target>
    struct TargetConvertedField<Target>
    {
        using Type = TargetConvertedFieldWithBases<Target, BaseSerializer>;
    };

    template<typename Target>
    struct PartialTargetConvertedField
    {
        template<typename... Cands>
        using Type = typename TargetConvertedField<Target, Cands...>::Type;
    };

    template<typename Target>
    using FieldSerializer = typename TargetConvertedField<Target>::Type;

    template<typename Target>
    using WeakFieldSerializer =
        typename typeutil::SerializationTrivialTypeList::Filter<
            inner::ShouldConvert<Target>::template Apply>::template Apply<
                PartialTargetConvertedField<Target>::template Type>;
}

#endif
