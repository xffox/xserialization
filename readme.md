A library with a serialization interface (`ISerializer` and `IDeserializer`) and macros
to describe serializable classes (`MT_CLASS`).

# Usage

With CMake:
```
find_package(XSerialization REQUIRED)
target_link_libraries(${TARGET_NAME} xserialization::xserialization)
```
`xserialization/serialization.hpp` is the main header.

# Serialization interface

`ISerializer` and `IDeserializer` are thought of as views of objects, exposing
the ability to traverse them as required for serialization. New implementations can
be added. The matching is type-driven, `SerializationTrait` is used for this.

# Class description

`MT_CLASS` describes serializable classes, `MT_FIELD` marks serializable fields.
Class attribute `MT_CLASS_ATTR_OPEN` allows to ignore unknown fields when written
to the class. Field attribute `MT_FIELD_ATTR_WEAK` allows weak type conversions
for the written types. `MT_FIELD_ATTR_OPTIONAL` allows the field to be missing.

`MT_CLASS` will have `ISerializer` and `IDeserializer` available.

## Note

Due to an implementation limitation aggregate initialization of the serializable
classes requires an additional empty base class initializer (see below).

The current way of relying on dynamic maps during static initialization is not
robust, exceptions cause aborts.

## Example

```
MT_CLASS(Point, MT_CLASS_ATTR_OPEN)
{
public:
    MT_FIELD(x, int, MT_FIELD_ATTR_WEAK);
    MT_FIELD(y, int, MT_FIELD_ATTR_WEAK);
    MT_FIELD(z, int, MT_FIELD_ATTR_WEAK, MT_FIELD_ATTR_OPTIONAL);
};
```

Aggregate initialization:
```
const Point p{{}, 42, 43}; // the first "{}" is required
```

# JSON

`json::JSON` class has `ISerializer` and `IDeserializer` implementations. JSON
serialization depends on the nlohmann JSON library. Enabled with the `USE_JSON`
CMake switch. And `find_package(nlohmann_json REQUIRED)` is required for this
to work.

Usage:
```
target_link_libraries(${TARGET_NAME} xserialization::xserialization_json)
```
`xserialization/json/json_serialization.hpp` is the main header for
the JSON serialization.

# Building

Uses CMake.

Can be as simple as
```
mkdir build; cd build
cmake -DUSE_JSON=on ..
cmake --build .
```
