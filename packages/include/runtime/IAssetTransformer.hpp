#pragma once
#include "core/Object.hpp"
namespace cube::runtime {
class IAssetTransformer : public core::Object {
public:
  virtual core::Object *transform(core::Object *object) = 0;
};
}; // namespace cube::runtime