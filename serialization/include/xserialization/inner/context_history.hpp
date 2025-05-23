#ifndef XSERIALIZATION_INNER_CONTEXTHISTORY_HPP
#define XSERIALIZATION_INNER_CONTEXTHISTORY_HPP

#include <type_traits>
#include <vector>
#include <utility>

#include "xserialization/context.hpp"
#include "xserialization/serializer.hpp"

namespace xserialization::inner::context_history
{
    class ContextHistory
    {
    private:
        struct State
        {
            std::vector<Context> contexts;
        };

        static auto &state()
        {
            thread_local static State state{};
            return state;
        }

        class ContextHistoryHandle
        {
            friend ContextHistory;
        public:
            ContextHistoryHandle(const ContextHistoryHandle&) = delete;
            ContextHistoryHandle(ContextHistoryHandle&&) = delete;
            ContextHistoryHandle &operator=(const ContextHistoryHandle&) = delete;
            ContextHistoryHandle &operator=(ContextHistoryHandle&&) = delete;

            ~ContextHistoryHandle()
            {
                state().contexts.pop_back();
            }

        private:
            ContextHistoryHandle() = default;
        };

    public:
        static ContextHistoryHandle push(Context context)
        {
            state().contexts.push_back(std::move(context));
            return ContextHistoryHandle();
        }

        static std::vector<Context> history()
        {
            return state().contexts;
        }
    };

    class ContextSpySerializer: public ISerializer
    {
    public:
        explicit ContextSpySerializer(ISerializer &serializer)
            :serializer(serializer)
        {}

        [[nodiscard]]
        Context::Type contextType() const override
        {
            return serializer.contextType();
        }

        void write(const IDeserializer &value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(Null value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(bool value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(char value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(signed char value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(unsigned char value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(short value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(unsigned short value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(int value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(unsigned int value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(long value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(unsigned long value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(long long value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(unsigned long long value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(float value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(double value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(long double value, const Context &context) override
        {
            writeValue(value, context);
        }
        void write(const std::string &value, const Context &context) override
        {
            writeValue(value, context);
        }

    private:
        template<typename T>
        void writeValue(const T &value, const Context &context)
        {
            const auto handle = ContextHistory::push(context);
            serializer.write(value, context);
        }

        ISerializer &serializer;
    };
}

#endif
