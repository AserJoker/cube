#pragma once
#include "Type.hpp"
namespace cube::reflection {
class DictType : public Type {
public:
  DictType();
  const char *getTypeName() const override;
};
} // namespace cube::reflection