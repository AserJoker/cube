#include "reflection/DictType.hpp"
namespace cube::reflection {
DictType::DictType() : Type(Type::Kind::DICT) {}
const char *DictType::getTypeName() const { return "dict"; }
}; // namespace cube::reflection