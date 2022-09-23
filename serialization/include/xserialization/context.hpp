#ifndef XSERIALIZATION_CONTEXT_HPP
#define XSERIALIZATION_CONTEXT_HPP

#include <cstddef>
#include <stdexcept>
#include <string>
#include <variant>
#include <utility>

namespace xserialization
{
    class Context
    {
    private:
        using Value = std::variant<std::monostate, std::size_t, std::string>;

    public:
        enum Type
        {
            TYPE_NONE = 0,
            TYPE_INDEX,
            TYPE_NAME,
        };

    public:
        Context() = default;
        Context(const char *name)
            :Context(std::string(name))
        {}
        Context(std::string name)
        :value(std::move(name))
        {}
        Context(std::size_t index)
        :value(index)
        {}

        [[nodiscard]]
        Type getType() const
        {
            static_assert(std::variant_size_v<Value>-1 == TYPE_NAME);
            return static_cast<Type>(value.index());
        }

        [[nodiscard]]
        std::size_t getIndex() const
        {
            if(const auto* index = std::get_if<std::size_t>(&value))
            {
                return *index;
            }
            throw std::logic_error("not an index context type");
        }

        [[nodiscard]]
        const std::string &getName() const
        {
            if(const auto *name = std::get_if<std::string>(&value))
            {
                return *name;
            }
            throw std::logic_error("not a name context type");
        }

        friend bool operator==(const Context &left, const Context &right)
        {
            return left.value == right.value;
        }
        friend bool operator!=(const Context &left, const Context &right)
        {
            return !(left == right);
        }

    private:
        Value value;
    };
}

#endif
