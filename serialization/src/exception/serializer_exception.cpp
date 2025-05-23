#include "xserialization/exception/serializer_exception.hpp"

#include <utility>
#include <sstream>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>
#include <cassert>

namespace xserialization::exception
{
    namespace
    {
        std::ostream &writeContext(std::ostream &stream, const Context &context)
        {
            switch(context.getType())
            {
            case Context::TYPE_NONE:
                stream<<"<none>";
                break;
            case Context::TYPE_INDEX:
                stream<<'['<<context.getIndex()<<']';
                break;
            case Context::TYPE_NAME:
                stream<<context.getName();
                break;
            default:
                assert(false);
                break;
            }
            return stream;
        }
    }

    std::string SerializerException::prepareContextedMessage(
            const Context &context, const std::vector<Context> &history,
            std::string_view msg)
    {
        std::stringstream messageStream;
        messageStream<<"context=";
        writeContext(messageStream, context);
        messageStream<<' '<<"history=";
        bool first = true;
        for(const auto &entry : history)
        {
            if(!first)
            {
                messageStream<<',';
            }
            else
            {
                first = false;
            }
            writeContext(messageStream, entry);
        }
        messageStream<<": "<<(!msg.empty() ? msg : "error");
        return std::move(messageStream).str();
    }
}
