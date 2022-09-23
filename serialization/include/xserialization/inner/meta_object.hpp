#ifndef XSERIALIZATION_INNER_METAOBJECT_HPP
#define XSERIALIZATION_INNER_METAOBJECT_HPP

#include <string>
#include <type_traits>
#include <unordered_map>
#include <tuple>
#include <utility>
#include <limits>
#include <cassert>

#include "xserialization/serializer.hpp"
#include "xserialization/deserializer.hpp"
#include "xserialization/context.hpp"
#include "xserialization/typeutil.hpp"
#include "xserialization/exception/serializer_exception.hpp"
#include "xserialization/exception/deserializer_exception.hpp"
#include "xserialization/inner/attribute.hpp"

namespace xserialization::inner
{
    class IField;

    class MetaObject
    {
    public:
        using FieldMap = std::unordered_map<std::string, IField*>;

    public:
        virtual ~MetaObject() = 0;

        [[nodiscard]]
        virtual const FieldMap &fields() const = 0;
        [[nodiscard]]
        virtual AttrMask attributes() const = 0;
        [[nodiscard]]
        virtual const std::string &className() const = 0;
    };

    class IField
    {
    public:
        virtual ~IField() = 0;

        [[nodiscard]]
        virtual AttrMask attributes() const = 0;

        virtual void visit(ISerializer &serializer, const MetaObject &object) = 0;

        virtual bool write(MetaObject&, const IDeserializer&)
        {
            return false;
        }
        virtual bool write(MetaObject&, bool)
        {
            return false;
        }
        virtual bool write(MetaObject&, char)
        {
            return false;
        }
        virtual bool write(MetaObject&, signed char)
        {
            return false;
        }
        virtual bool write(MetaObject&, unsigned char)
        {
            return false;
        }
        virtual bool write(MetaObject&, short)
        {
            return false;
        }
        virtual bool write(MetaObject&, unsigned short)
        {
            return false;
        }
        virtual bool write(MetaObject&, int)
        {
            return false;
        }
        virtual bool write(MetaObject&, unsigned int)
        {
            return false;
        }
        virtual bool write(MetaObject&, long)
        {
            return false;
        }
        virtual bool write(MetaObject&, unsigned long)
        {
            return false;
        }
        virtual bool write(MetaObject&, long long)
        {
            return false;
        }
        virtual bool write(MetaObject&, unsigned long long)
        {
            return false;
        }
        virtual bool write(MetaObject&, float)
        {
            return false;
        }
        virtual bool write(MetaObject&, double)
        {
            return false;
        }
        virtual bool write(MetaObject&, long double)
        {
            return false;
        }
        virtual bool write(MetaObject&, const std::string&)
        {
            return false;
        }
    };

    namespace inner
    {
        template<typename T>
        inline constexpr bool IsWeakConvertible =
            (std::is_arithmetic_v<T> && !std::is_same_v<T, bool>);
    }

    template<typename Target, typename Cand, typename = void>
    class BaseConvertedField: public virtual IField
    {};

    template<typename Target, typename Cand>
    class BaseConvertedField<Target, Cand,
          std::enable_if_t<
              !std::is_same_v<Target, Cand> &&
              inner::IsWeakConvertible<Target> && inner::IsWeakConvertible<Cand> &&
              std::is_convertible_v<Cand, Target> &&
              std::is_floating_point_v<Target> >= std::is_floating_point_v<Cand>>>:
                  public virtual IField
    {
    public:
        bool write(MetaObject &object, Cand value) override
        {
            bool valid = true;
            if constexpr(std::is_signed_v<Target> == std::is_signed_v<Cand> ||
                    std::is_floating_point_v<Target>)
            {
                valid = (value >= std::numeric_limits<Target>::lowest() &&
                        value <= std::numeric_limits<Target>::max());
            }
            else if constexpr(std::is_unsigned_v<Target>)
            {
                valid = (value >= 0 &&
                        static_cast<std::make_unsigned_t<Cand>>(value) <=
                        std::numeric_limits<Target>::max());
            }
            else
            {
                valid = (value <= static_cast<std::make_unsigned_t<Target>>(
                            std::numeric_limits<Target>::max()));
            }
            if(!valid)
            {
                return false;
            }
            return static_cast<IField&>(*this).write(object, static_cast<Target>(value));
        }
    };

    template<typename Target, typename... Cands>
    class TargetConvertedField: public BaseConvertedField<Target, Cands>...
    {
    public:
        using IField::write;
    };

    template<typename Target>
    struct PartialTargetConvertedField
    {
        template<typename... Cands>
        using Type = TargetConvertedField<Target, Cands...>;
    };

    template<typename Target>
    using WeakField =
        typename typeutil::SerializationTrivialTypes<
            PartialTargetConvertedField<Target>::template Type>::Type;

    template<class T, AttrMask classAttributes>
    class MetaObjectBase: public MetaObject
    {
    public:
        ~MetaObjectBase() override = 0;

        [[nodiscard]]
        const FieldMap &fields() const override
        {
            return MetaObjectBase::getFields();
        }

        [[nodiscard]]
        AttrMask attributes() const override
        {
            return classAttributes;
        }

    protected:
        using BaseClass = T;

    protected:
        static FieldMap &getFields()
        {
            static FieldMap fields;
            return fields;
        }

        static bool addField(const std::string &name, IField &field)
        {
            return getFields().insert(std::make_pair(name, &field)).second;
        }
    };

    template<class T, AttrMask classAttributes>
    MetaObjectBase<T, classAttributes>::~MetaObjectBase() = default;
}

#endif
