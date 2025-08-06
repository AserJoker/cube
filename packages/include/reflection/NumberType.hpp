#pragma once
#include "Type.hpp"
namespace cube::reflection {
class NumberType : public Type {
public:
  NumberType();
  const char *getTypeName() const override;
};
} // namespace cube::reflection