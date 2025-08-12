#pragma once
#include "IGeometory.hpp"
#include "Material.hpp"
#include "core/Object.hpp"
namespace cube::render {
class IMesh : public core::Object {
public:
  virtual IGeometory *getGeometory() = 0;
  virtual Material *getMaterial() = 0;
};
} // namespace cube::render