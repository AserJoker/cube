#include "reflection/Type.hpp"
namespace cube::reflection {
Type::Type(const Type::Kind &kind) : _kind(kind) {}

const Type::Kind &Type::getKind() const { return _kind; }
}; // namespace cube::reflection