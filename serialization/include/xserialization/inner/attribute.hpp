#ifndef XSERIALIZATION_INNER_FIELDATTRIBUTE_HPP
#define XSERIALIZATION_INNER_FIELDATTRIBUTE_HPP

namespace xserialization::inner
{
    using AttrMask = unsigned int;

    enum class FieldAttribute: inner::AttrMask
    {
        WEAK = 1,
        OPTIONAL = 2,
    };

    enum class ClassAttribute: inner::AttrMask
    {
        OPEN = 1,
    };
}

#endif
