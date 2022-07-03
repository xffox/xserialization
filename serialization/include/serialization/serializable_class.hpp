#ifndef SERIALIZATION_SERIALIZABLECLASS_HPP
#define SERIALIZATION_SERIALIZABLECLASS_HPP

#include "serialization/inner/meta_object.hpp"
#include "serialization/typeutil.hpp"
#include "serialization/util.hpp"

#define _FIELD_DEF(type, name) \
    struct _##name##FieldInitializerType \
    { \
        _##name##FieldInitializerType() \
        { \
            static _##name##FieldType field; \
        } \
    } _##name##FieldInitializer; \
    type name

#define _SERIALIZABLE_FIELD(type, name, base) \
    struct _##name##FieldType: public base \
    { \
        using base::write; \
        _##name##FieldType() \
        { \
            BaseClass::addField(#name, *this); \
        } \
        void visit(serialization::ISerializer &serializer, \
            const serialization::inner::MetaObject &object) override \
        { \
            const auto &cur = static_cast<const BaseClass&>(object); \
            const auto &value = cur.name; \
            const serialization::Context context(#name); \
            serialization::util::visitValue(serializer, value, context); \
        } \
        bool write(serialization::inner::MetaObject &object, \
                serialization::typeutil::WriteType<type> value) override \
        { \
            auto &cur = static_cast<BaseClass&>(object); \
            return serialization::util::writeValue(cur.name, value); \
        } \
    }; \
    _FIELD_DEF(type, name)

// TODO: probably add class name field
#define SERIALIZABLE_CLASS(cl) \
    class _##cl##ClassName \
    { \
    public: \
        static const char *getClassName() \
        { \
            return #cl; \
        } \
    }; \
    class cl: public serialization::inner::MetaObjectBase<cl>, public _##cl##ClassName

#define SERIALIZABLE_FIELD(type, name) \
    _SERIALIZABLE_FIELD(type, name, serialization::inner::IField)

#define SERIALIZABLE_WEAK_FIELD(type, name) \
    _SERIALIZABLE_FIELD(type, name, serialization::inner::WeakField<type>)

#endif
