#pragma once
#include "Type.hpp"
namespace cube::reflection {
class IntegerType : public Type {
public:
  IntegerType();
  const char *getTypeName() const override;
};
} // namespace cube::reflection