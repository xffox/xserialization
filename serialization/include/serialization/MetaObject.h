#ifndef SERIALIZATION_METAOBJECT_H
#define SERIALIZATION_METAOBJECT_H

#include <cassert>
#include <string>
#include <map>
#include <memory>

#include "serialization/ISerializer.h"
#include "serialization/Context.h"
#include "serialization/serialization.h"

namespace serialization
{
    class MetaObject
    {
    public:
        virtual ~MetaObject(){}

        virtual void visit(ISerializer &serializer) const = 0;
        virtual std::auto_ptr<ISerializer> beginCollection(
            const std::string &name) = 0;
        virtual bool writeNull(const std::string &name) = 0;
        virtual bool write(const std::string &name, bool value) = 0;
        virtual bool write(const std::string &name, char value) = 0;
        virtual bool write(const std::string &name, signed char value) = 0;
        virtual bool write(const std::string &name, unsigned char value) = 0;
        virtual bool write(const std::string &name, short value) = 0;
        virtual bool write(const std::string &name, unsigned short value) = 0;
        virtual bool write(const std::string &name, int value) = 0;
        virtual bool write(const std::string &name, unsigned int value) = 0;
        virtual bool write(const std::string &name, long value) = 0;
        virtual bool write(const std::string &name, unsigned long value) = 0;
        virtual bool write(const std::string &name, long long value) = 0;
        virtual bool write(const std::string &name, unsigned long long value) = 0;
        virtual bool write(const std::string &name, float value) = 0;
        virtual bool write(const std::string &name, double value) = 0;
        virtual bool write(const std::string &name, long double value) = 0;
        virtual bool write(const std::string &name, const std::string &value) = 0;

    protected:
        class IField
        {
        public:
            virtual ~IField(){}

            virtual void visit(ISerializer &serializer,
                const MetaObject &object) = 0;

            virtual std::auto_ptr<ISerializer> beginCollection(
                MetaObject &object) = 0;

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
            virtual bool write(MetaObject&, std::string)
            {
                return false;
            }
        };
        typedef std::map<std::string, IField*> FieldMap;
    };

    template<class T>
    class MetaObjectBase: public MetaObject
    {
    public:
        virtual void visit(ISerializer &serializer) const;
        virtual bool writeNull(const std::string &name);
        virtual std::auto_ptr<ISerializer> beginCollection(
            const std::string &name);
        virtual bool write(const std::string &name, bool value)
        { return writeValue(name, value); }
        virtual bool write(const std::string &name, char value)
        { return writeValue(name, value); }
        virtual bool write(const std::string &name, signed char value)
        { return writeValue(name, value); }
        virtual bool write(const std::string &name, unsigned char value)
        { return writeValue(name, value); }
        virtual bool write(const std::string &name, short value)
        { return writeValue(name, value); }
        virtual bool write(const std::string &name, unsigned short value)
        { return writeValue(name, value); }
        virtual bool write(const std::string &name, int value)
        { return writeValue(name, value); }
        virtual bool write(const std::string &name, unsigned int value)
        { return writeValue(name, value); }
        virtual bool write(const std::string &name, long value)
        { return writeValue(name, value); }
        virtual bool write(const std::string &name, unsigned long value)
        { return writeValue(name, value); }
        virtual bool write(const std::string &name, long long value)
        { return writeValue(name, value); }
        virtual bool write(const std::string &name, unsigned long long value)
        { return writeValue(name, value); }
        virtual bool write(const std::string &name, float value)
        { return writeValue(name, value); }
        virtual bool write(const std::string &name, double value)
        { return writeValue(name, value); }
        virtual bool write(const std::string &name, long double value)
        { return writeValue(name, value); }
        virtual bool write(const std::string &name, const std::string &value)
        { return writeValue(name, value); }

    protected:
        typedef T BaseClass;

    protected:
        static FieldMap &getFields()
        {
            static FieldMap fields;
            return fields;
        }

        static void addField(const std::string &name, IField &field)
        {
            // TODO: assert uniquety
            getFields().insert(std::pair<std::string, IField*>(name, &field));
        }

        template<typename V>
        bool writeValue(const std::string &name, const V &value);
    };

    template<typename T>
    bool MetaObjectBase<T>::writeNull(const std::string&)
    {
        return false;
    }

    template<typename T>
    void MetaObjectBase<T>::visit(ISerializer &serializer) const
    {
        for(typename FieldMap::const_iterator iter = getFields().begin(),
            endIter = getFields().end(); iter != endIter; ++iter)
        {
            iter->second->visit(serializer, *this);
        }
    }

    template<typename T>
    std::auto_ptr<ISerializer> MetaObjectBase<T>::beginCollection(
        const std::string &name)
    {
        FieldMap::iterator iter = getFields().find(name);
        if(iter != getFields().end())
            return iter->second->beginCollection(*this);
        return std::auto_ptr<ISerializer>();
    }

    template<typename T>
    template<typename V>
    bool MetaObjectBase<T>::writeValue(const std::string &name, const V &value)
    {
        FieldMap::iterator iter = getFields().find(name);
        if(iter != getFields().end())
            return iter->second->write(*this, value);
        return false;
    }
}

#define FIELD_DEF(type, name) \
    struct name##FieldInitializerType \
    { \
        name##FieldInitializerType() \
        { \
            static name##FieldType field; \
        } \
    } name##FieldInitializer; \
    type name

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
    class cl: public serialization::MetaObjectBase<cl>, public _##cl##ClassName

#define SERIALIZABLE_FIELD(type, name) \
    struct name##FieldType: \
              public serialization::MetaObject::IField \
    { \
        name##FieldType() \
        { \
            BaseClass::addField(#name, *this); \
        } \
        virtual void visit(serialization::ISerializer &serializer, \
            const serialization::MetaObject &object) \
        { \
            serialization::write(serializer, \
                static_cast<const BaseClass&>(object).name, #name); \
        } \
        virtual bool write(serialization::MetaObject &object, type value) \
        { \
            static_cast<BaseClass&>(object).name = value; \
            return true; \
        } \
        virtual std::auto_ptr<serialization::ISerializer> beginCollection( \
            MetaObject &object) \
        { \
            return serialization::beginCollection( \
                static_cast<BaseClass&>(object).name); \
        } \
    }; \
    FIELD_DEF(type, name)

#endif
