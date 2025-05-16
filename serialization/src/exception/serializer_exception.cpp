#include "xserialization/exception/serializer_exception.hpp"

#include <utility>
#include <sstream>
#include <string>
#include <string_view>
#include <cassert>

namespace xserialization::exception
{
    std::string SerializerException::prepareContextedMessage(
            const Context &context, std::string_view msg)
    {
        std::stringstream messageStream;
        messageStream<<"context=";
        switch(context.getType())
        {
            case Context::TYPE_NONE:
                messageStream<<"<none>";
                break;
            case Context::TYPE_INDEX:
                messageStream<<'['<<context.getIndex()<<']';
                break;
            case Context::TYPE_NAME:
                messageStream<<context.getName();
                break;
            default:
                assert(false);
                break;
        }
        messageStream<<": "<<(!msg.empty() ? msg : "error");
        return std::move(messageStream).str();
    }
}
