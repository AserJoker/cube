#pragma once
#include "Type.hpp"
namespace cube::reflection {
class NullType : public Type {
public:
  NullType();
  const char *getTypeName() const override;
};
} // namespace cube::reflection