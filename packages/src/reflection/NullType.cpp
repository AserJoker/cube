#include "reflection/NullType.hpp"
namespace cube::reflection {
NullType::NullType() : Type(Type::Kind::NIL) {}
const char *NullType::getTypeName() const { return "null"; }
} // namespace cube::reflection