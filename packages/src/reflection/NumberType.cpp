#include "reflection/NumberType.hpp"
namespace cube::reflection {
NumberType::NumberType() : Type(Type::Kind::NUMBER) {}
const char *NumberType::getTypeName() const { return "number"; }
}; // namespace cube::reflection