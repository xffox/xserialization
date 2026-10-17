#ifndef XSERIALIZATION_INNER_METAFIELD_HPP
#define XSERIALIZATION_INNER_METAFIELD_HPP

#include <type_traits>

#include "xserialization/context.hpp"
#include "xserialization/serializer.hpp"
#include "xserialization/deserializer.hpp"
#include "xserialization/typeutil.hpp"
#include "xserialization/valutil.hpp"
#include "xserialization/exception/serializer_exception.hpp"
#include "xserialization/inner/wrap_serializer.hpp"
#include "xserialization/inner/attribute.hpp"

namespace xserialization
{
    class IDeserializer;
}

namespace xserialization::inner::field
{
    class IField
    {
    public:
        virtual ~IField() = default;

        [[nodiscard]]
        virtual AttrMask attributes() const = 0;

        // TODO: remove the allocation
        virtual void write(void *inst, const IDeserializer &value) const = 0;
        virtual void visit(ISerializer &serializer, const void *inst) const = 0;
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
    class BaseConvertedFieldSerializer: public virtual WrapSerializer
    {
    public:
        using WrapSerializer::WrapSerializer;

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
        using Cands::Cands...;
        using ISerializer::write;

    protected:
        void visitWrapped(ISerializer &serializer,
            const IDeserializer &deserializer) const override
        {
            TargetConvertedFieldWithBases wrappedSerializer(serializer);
            deserializer.visit(wrappedSerializer);
        }
    };

    template<typename Target, typename... Cands>
    struct TargetConvertedField
    {
        using Type = TargetConvertedFieldWithBases<Target, BaseConvertedFieldSerializer<Target, Cands>...>;
    };
    template<typename Target>
    struct TargetConvertedField<Target>
    {
        using Type = TargetConvertedFieldWithBases<Target, WrapSerializer>;
    };

    template<typename Target>
    struct PartialTargetConvertedField
    {
        template<typename... Cands>
        using Type = typename TargetConvertedField<Target, Cands...>::Type;
    };

    template<typename Target>
    using WeakFieldSerializer =
        typename typeutil::SerializationTrivialTypeList::Filter<
            inner::ShouldConvert<Target>::template Apply>::template Apply<
                PartialTargetConvertedField<Target>::template Type>;
}

#endif
