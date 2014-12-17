#ifndef BASE_VARIANT_H
#define BASE_VARIANT_H

#include <cassert>
#include <stdexcept>

#include "variant/TypeDescr.h"

namespace variant
{
    template<class Type = TypeDescr>
    class BaseVariant
    {
        template<typename T>
        friend bool ::operator==(const BaseVariant<T> &left,
            const BaseVariant<T> &right);
        template<typename T>
        friend bool equalType(const BaseVariant<T> &left,
            const BaseVariant<T> &right);
    public:
        template<typename T>
        explicit BaseVariant(const T &value);
        BaseVariant(const BaseVariant &that);
        virtual ~BaseVariant();

        BaseVariant &operator=(const BaseVariant &that);

        template<typename T>
        static BaseVariant make(const T &value);

        template<typename T>
        bool toValue(T &value) const;

        template<typename T>
        T &getValue();

        template<typename T>
        const T &getValue() const
        {
            return const_cast<BaseVariant*>(this)->getValue<T>();
        }

        template<typename T>
        bool isType() const;

    protected:
        void *getData()
        {
            return data;
        }

        const void *getData() const
        {
            return const_cast<BaseVariant*>(this)->getData();
        }

        const Type &getType() const
        {
            assert(type);
            return *type;
        }

    private:
        const Type *type;
        // TODO: auto_ptr
        void *data;
    };

    typedef BaseVariant<TypeDescr> Variant;

    template<class Type>
    BaseVariant<Type>::BaseVariant(const BaseVariant &that)
        :type(that.type), data(0)
    {
        assert(type);
        assert(that.data);
        data = type->clone(that.data);
        assert(data);
    }

    template<class Type>
    BaseVariant<Type>::~BaseVariant()
    {
        assert(type);
        assert(data);
        type->destruct(data);
    }

    template<class Type>
    BaseVariant<Type> &BaseVariant<Type>::operator=(const BaseVariant &that)
    {
        assert(type);
        type->destruct(data);
        assert(that.type);
        assert(that.data);
        type = that.type;
        data = type->clone(that.data);
        return *this;
    }

    template<class Type>
    template<typename T>
    BaseVariant<Type>::BaseVariant(const T &value)
        :type(&Type::template type<T>()), data(0)
    {
        assert(type);
        data = type->construct();
        assert(data);
        *static_cast<T*>(data) = value;
    }

    template<class Type>
    template<typename T>
    BaseVariant<Type> BaseVariant<Type>::make(const T &value)
    {
        return BaseVariant(value);
    }

    template<class Type>
    template<typename T>
    T &BaseVariant<Type>::getValue()
    {
        if(isType<T>())
        {
            assert(data);
            return *static_cast<T*>(data);
        }
        throw std::logic_error("invalid variant type");
    }

    template<class Type>
    template<typename T>
    bool BaseVariant<Type>::toValue(T &value) const
    {
        if(isType<T>())
        {
            assert(data);
            value = *static_cast<const T*>(data);
            return true;
        }
        return false;
    }

    template<class Type>
    template<typename T>
    bool BaseVariant<Type>::isType() const
    {
        assert(type);
        return Type::template type<T>() == *type;
    }

    template<class Type>
    inline bool equalType(const BaseVariant<Type> &left,
        const BaseVariant<Type> &right)
    {
        assert(left.type);
        assert(right.type);
        return *left.type == *right.type;
    }
}

template<class Type>
inline bool operator==(const variant::BaseVariant<Type> &left,
    const variant::BaseVariant<Type> &right)
{
    if(equalType(left, right))
    {
        assert(left.type);
        return left.type->compare(left.data, right.data);
    }
    return false;
}

template<class Type>
inline bool operator!=(const variant::BaseVariant<Type> &left,
    const variant::BaseVariant<Type> &right)
{
    return !(left == right);
}

#endif
