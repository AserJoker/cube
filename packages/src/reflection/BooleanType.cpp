#include "reflection/BooleanType.hpp"
namespace cube::reflection {
BooleanType::BooleanType() : Type(Type::Kind::BOOLEAN) {}

const char *BooleanType::getTypeName() const { return "boolean"; }
}; // namespace cube::reflection