#ifndef SERIALIZATION_CONTEXT_H
#define SERIALIZATION_CONTEXT_H

#include <cstddef>
#include <stdexcept>
#include <string>

namespace serialization
{
    class Context
    {
    public:
        enum Type
        {
            TYPE_NONE,
            TYPE_NAME,
            TYPE_INDEX
        };

    public:
        Context()
        :type(TYPE_NONE), index(), name()
        {}
        Context(const char *name)
        :type(TYPE_NAME), index(), name(name)
        {}
        Context(const std::string &name)
        :type(TYPE_NAME), index(), name(name)
        {}
        Context(std::size_t index)
        :type(TYPE_INDEX), index(index), name()
        {}

        Type getType() const
        {
            return type;
        }

        std::size_t getIndex() const
        {
            if(type == TYPE_INDEX)
                return index;
            throw std::logic_error("not an index context type");
        }

        const std::string &getName() const
        {
            if(type == TYPE_NAME)
                return name;
            throw std::logic_error("not a name context type");
        }

    private:
        Type type;
        std::size_t index;
        std::string name;
    };
}

#endif
