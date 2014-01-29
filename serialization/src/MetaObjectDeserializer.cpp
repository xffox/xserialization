#include "serialization/MetaObjectDeserializer.h"

#include "serialization/ISerializer.h"
#include "serialization/Context.h"
#include "serialization/MetaObject.h"
#include "serialization/exception/SerializationException.h"

namespace serialization
{
    void MetaObjectDeserializer::visit(ISerializer &serializer,
        const serialization::Context &context) const
    {
        std::auto_ptr<serialization::ISerializer> s =
            serializer.beginCollection(Context::TYPE_NAME, context);
        if(s.get())
            object.visit(*s);
        else
            throw exception::SerializationException(context);
    }
}
