#include "reflection/IntegerType.hpp"
namespace cube::reflection {
IntegerType::IntegerType() : Type(Type::Kind::INTEGER) {}
const char *IntegerType::getTypeName() const { return "integer"; }
}; // namespace cube::reflection