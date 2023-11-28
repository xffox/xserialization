#include "xserialization/json/json_deserializer.hpp"

#include <iterator>
#include <optional>
#include <cassert>

#include "xserialization/context.hpp"
#include "xserialization/serializer.hpp"
#include "xserialization/null.hpp"
#include "xserialization/typeutil.hpp"
#include "xserialization/valutil.hpp"
#include "xserialization/exception/serializer_exception.hpp"
#include "xserialization/exception/deserializer_exception.hpp"
#include "inner/json_impl.hpp"

namespace xserialization::json
{
    namespace
    {
        template<typename Target, typename Src, typename F>
        bool tryConsumeValue(const Src &val, F consume)
        {
            if(!valutil::canAssign<Target>(val))
            {
                return false;
            }
            try
            {
                consume(static_cast<Target>(val));
                return true;
            }
            catch(const exception::TypeSerializerException&)
            {
                return false;
            }
        }

        template<typename... Ts>
        struct TypeConsumer
        {
            template<typename Src, typename F>
            static bool tryConsume(const Src &val, F consume)
            {
                return (tryConsumeValue<Ts>(val, consume) || ...);
            }
        };

        template<typename Src, typename F>
        void consumeIntegerValue(const Src &val, F consume)
        {
            if(!(typeutil::SerializationUnsignedIntegerTypes<TypeConsumer>::Type::tryConsume(val, consume) ||
                        typeutil::SerializationSignedIntegerTypes<TypeConsumer>::Type::tryConsume(val, consume) ||
                        typeutil::SerializationFloatingPointTypes<TypeConsumer>::Type::tryConsume(val, consume)))
            {
                throw exception::DeserializerException("no matching type to write");
            }
        }

        template<typename Src, typename F>
        void consumeFloatingPointValue(const Src &val, F consume)
        {
            if(!typeutil::SerializationFloatingPointTypes<TypeConsumer>::Type::tryConsume(val, consume))
            {
                throw exception::DeserializerException("no matching type to write");
            }
        }

        template<typename F, typename E>
        void consumeValue(const inner::JSONImpl &value, F consume, E invalid)
        {
            switch(value.type())
            {
            case inner::JSONImpl::value_t::boolean:
                consume(value.get<inner::JSONImpl::boolean_t>());
                break;
            case inner::JSONImpl::value_t::number_integer:
                consumeIntegerValue(value.get<inner::JSONImpl::number_integer_t>(), consume);
                break;
            case inner::JSONImpl::value_t::number_unsigned:
                consumeIntegerValue(value.get<inner::JSONImpl::number_unsigned_t>(), consume);
                break;
            case inner::JSONImpl::value_t::number_float:
                consumeFloatingPointValue(value.get<inner::JSONImpl::number_float_t>(), consume);
                break;
            case inner::JSONImpl::value_t::string:
                consume(value.get<inner::JSONImpl::string_t>());
                break;
            case inner::JSONImpl::value_t::null:
                consume(Null{});
                break;
            default:
                invalid();
                break;
            }
        }
    }

    void JSONDeserializer::visit(ISerializer &serializer) const
    {
        auto writeValue =
            [&serializer](const inner::JSONImpl &value, const Context &context){
                consumeValue(value, [&serializer, &context](const auto &v){
                            serializer.write(v, context);
                        },
                        [&serializer, &context, &value](){
                            serializer.write(
                                    JSONDeserializer(inner::toJSONOpaque(value)),
                                    context);
                        });
        };
        const auto &value = inner::toJSONImpl(json);
        switch(contextType())
        {
        case Context::TYPE_NAME:
            for(const auto &el : value.items())
            {
                writeValue(el.value(), el.key());
            }
            break;
        case Context::TYPE_INDEX:
            for(auto iter = std::begin(value); iter != std::end(value); ++iter)
            {
                writeValue(*iter, iter-std::begin(value));
            }
            break;
        case Context::TYPE_NONE:
            writeValue(value, {});
            break;
        default:
            assert(false);
            break;
        }
    }
}
