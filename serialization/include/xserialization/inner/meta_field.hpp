#ifndef XSERIALIZATION_INNER_METAFIELD_HPP
#define XSERIALIZATION_INNER_METAFIELD_HPP

#include <type_traits>
#include <string>

#include "xserialization/serializer.hpp"
#include "xserialization/deserializer.hpp"
#include "xserialization/context.hpp"
#include "xserialization/typeutil.hpp"
#include "xserialization/valutil.hpp"
#include "xserialization/inner/attribute.hpp"

namespace xserialization::inner::field
{
    template<typename Cl>
    class IField
    {
    public:
        virtual ~IField() = 0;

        [[nodiscard]]
        virtual AttrMask attributes() const = 0;

        virtual void visit(ISerializer &serializer, const Cl &object) = 0;

        virtual bool write(Cl&, const IDeserializer&)
        {
            return false;
        }
        virtual bool write(Cl&, bool)
        {
            return false;
        }
        virtual bool write(Cl&, char)
        {
            return false;
        }
        virtual bool write(Cl&, signed char)
        {
            return false;
        }
        virtual bool write(Cl&, unsigned char)
        {
            return false;
        }
        virtual bool write(Cl&, short)
        {
            return false;
        }
        virtual bool write(Cl&, unsigned short)
        {
            return false;
        }
        virtual bool write(Cl&, int)
        {
            return false;
        }
        virtual bool write(Cl&, unsigned int)
        {
            return false;
        }
        virtual bool write(Cl&, long)
        {
            return false;
        }
        virtual bool write(Cl&, unsigned long)
        {
            return false;
        }
        virtual bool write(Cl&, long long)
        {
            return false;
        }
        virtual bool write(Cl&, unsigned long long)
        {
            return false;
        }
        virtual bool write(Cl&, float)
        {
            return false;
        }
        virtual bool write(Cl&, double)
        {
            return false;
        }
        virtual bool write(Cl&, long double)
        {
            return false;
        }
        virtual bool write(Cl&, const std::string&)
        {
            return false;
        }
    };

    template<typename Cl>
    IField<Cl>::~IField() = default;

    namespace inner
    {
        template<typename T>
        inline constexpr bool IsWeakConvertible =
            (std::is_arithmetic_v<T> && !std::is_same_v<T, bool>);
    }

    template<typename Cl, typename Target, typename Cand, typename = void>
    class BaseConvertedField: public virtual IField<Cl>
    {};

    template<typename Cl, typename Target, typename Cand>
    class BaseConvertedField<Cl, Target, Cand,
          std::enable_if_t<
              !std::is_same_v<Target, Cand> &&
              inner::IsWeakConvertible<Target> && inner::IsWeakConvertible<Cand> &&
              std::is_convertible_v<Cand, Target> &&
              std::is_floating_point_v<Target> >= std::is_floating_point_v<Cand>>>:
                  public virtual IField<Cl>
    {
    public:
        bool write(Cl &object, Cand value) override
        {
            if(!valutil::canAssign<Target>(value))
            {
                return false;
            }
            return static_cast<IField<Cl>&>(*this).write(object, static_cast<Target>(value));
        }
    };

    template<typename Cl, typename Target, typename... Cands>
    class TargetConvertedField: public BaseConvertedField<Cl, Target, Cands>...
    {
    public:
        using IField<Cl>::write;
    };

    template<typename Cl, typename Target>
    struct PartialTargetConvertedField
    {
        template<typename... Cands>
        using Type = TargetConvertedField<Cl, Target, Cands...>;
    };

    template<typename Cl, typename Target>
    using WeakField =
        typename typeutil::SerializationTrivialTypes<
            PartialTargetConvertedField<Cl, Target>::template Type>::Type;
}

#endif
