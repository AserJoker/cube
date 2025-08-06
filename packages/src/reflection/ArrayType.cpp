#include "reflection/ArrayType.hpp"
namespace cube::reflection {
ArrayType::ArrayType() : Type(Type::Kind::ARRAY) {}
const char *ArrayType::getTypeName() const { return "array"; }
}; // namespace cube::reflection