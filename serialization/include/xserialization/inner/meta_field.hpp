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
    template<typename Cl>
    class IField
    {
    public:
        virtual ~IField() = 0;

        [[nodiscard]]
        virtual AttrMask attributes() const = 0;

        // TODO: remove the allocation
        [[nodiscard]]
        virtual std::unique_ptr<ISerializer> makeSerializer(Cl&) const = 0;
        [[nodiscard]]
        virtual std::unique_ptr<IDeserializer> makeDeserializer(const Cl&) const = 0;
    };

    template<typename Cl>
    IField<Cl>::~IField() = default;

    template<typename T>
    class FieldSerializer: public BaseSerializer
    {
    public:
        explicit FieldSerializer(T &dst)
            :dst(dst)
        {}

        [[nodiscard]]
        Context::Type contextType() const override
        {
            return Context::TYPE_NONE;
        }

        using BaseSerializer::write;

        void write(
                typeutil::WriteType<T> value,
                const Context &context) override
        {
            if(!xserialization::util::writeValue(dst, value))
            {
                throw exception::TypeSerializerException(context, "invalid field write");
            }
        }

    protected:
        T &dst;
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
    }

    template<typename Target, typename Cand, typename = void>
    class BaseConvertedFieldSerializer: public virtual FieldSerializer<Target>
    {
        using FieldSerializer<Target>::FieldSerializer;
    };

    template<typename Target, typename Cand>
    class BaseConvertedFieldSerializer<Target, Cand,
          std::enable_if_t<
              !std::is_same_v<Target, Cand> &&
              inner::IsWeakConvertible<Target> && inner::IsWeakConvertible<Cand> &&
              std::is_convertible_v<Cand, Target> &&
              std::is_floating_point_v<Target> >= std::is_floating_point_v<Cand>>>:
                  public virtual FieldSerializer<Target>
    {
    public:
        using FieldSerializer<Target>::FieldSerializer;

        using FieldSerializer<Target>::write;

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
            return static_cast<FieldSerializer<Target>&>(*this).write(static_cast<Target>(value), context);
        }
    };

    template<typename Target, typename... Cands>
    class TargetConvertedField: public BaseConvertedFieldSerializer<Target, Cands>...
    {
    public:
        using BaseConvertedFieldSerializer<Target, Cands>::BaseConvertedFieldSerializer...;

        using FieldSerializer<Target>::write;
    };

    template<typename Target>
    struct PartialTargetConvertedField
    {
        template<typename... Cands>
        using Type = TargetConvertedField<Target, Cands...>;
    };

    template<typename Target>
    using WeakFieldSerializer =
        typename typeutil::SerializationTrivialTypes<
            PartialTargetConvertedField<Target>::template Type>::Type;
}

#endif
