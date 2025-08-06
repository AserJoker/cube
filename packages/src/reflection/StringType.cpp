#include "reflection/StringType.hpp"
namespace cube::reflection {
StringType::StringType() : Type(Type::Kind::STRING) {}

const char *StringType::getTypeName() const { return "string"; }
}; // namespace cube::reflection