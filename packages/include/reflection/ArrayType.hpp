#pragma once
#include "Type.hpp"
namespace cube::reflection {
class ArrayType : public Type {
public:
  ArrayType();
  const char *getTypeName() const override;
};
} // namespace cube::reflection