#pragma once
#include "Type.hpp"
namespace cube::reflection {
class StringType : public Type {
public:
  StringType();
  const char *getTypeName() const override;
};
} // namespace cube::reflection