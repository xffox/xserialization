#ifndef VARIANT_TYPEDESCR_H
#define VARIANT_TYPEDESCR_H

namespace variant
{
    class TypeDescr
    {
    public:
        virtual ~TypeDescr() = default;

        virtual void *construct() const = 0;
        virtual void destruct(void *data) const = 0;
        virtual void *clone(const void *data) const = 0;
        virtual bool compare(const void *left, const void *right) const = 0;

        template<typename T>
        static const TypeDescr &type()
        {
            static TypeImpl<T> instance;
            return instance;
        }

    private:
        template<typename T>
        class TypeImpl;

    private:
        TypeDescr() = default;
    };

    template<typename T>
    class TypeDescr::TypeImpl: public TypeDescr
    {
    public:
        virtual void *construct() const;
        virtual void destruct(void *data) const;
        virtual void *clone(const void *data) const;
        virtual bool compare(const void *left, const void *right) const;
    };

    template<typename T>
    void *TypeDescr::TypeImpl<T>::construct() const
    {
        return new T();
    }

    template<typename T>
    void TypeDescr::TypeImpl<T>::destruct(void *data) const
    {
        delete static_cast<T*>(data);
    }

    template<typename T>
    void *TypeDescr::TypeImpl<T>::clone(const void *data) const
    {
        assert(data);
        return new T(*static_cast<const T*>(data));
    }

    template<typename T>
    bool TypeDescr::TypeImpl<T>::compare(const void *left, const void *right) const
    {
        assert(left);
        assert(right);
        return *static_cast<const T*>(left) == *static_cast<const T*>(right);
    }
}

inline bool operator==(const variant::TypeDescr &left,
    const variant::TypeDescr &right)
{
    return &left == &right;
}

inline bool operator!=(const variant::TypeDescr &left,
    const variant::TypeDescr &right)
{
    return !(left == right);
}

#endif
