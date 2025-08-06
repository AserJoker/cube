#pragma once
#include "IGeometory.hpp"
#include "IMaterial.hpp"
#include "core/Object.hpp"
namespace cube::render {
class IMesh : public core::Object {
public:
  virtual IGeometory *getGeometory() = 0;
  virtual IMaterial *getMerial() = 0;
};
} // namespace cube::render