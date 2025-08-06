#pragma once
#include "Type.hpp"
namespace cube::reflection {
class BooleanType : public Type {
public:
  BooleanType();
  const char *getTypeName() const override;
};
} // namespace cube::reflection