#pragma once
#include "core/Object.hpp"
#include <glm/glm.hpp>
namespace cube::render {
class ITexture : public core::Object {
public:
  enum class FORMAT { RGB24, RGBA32, GRAY };

public:
  virtual ITexture *setRepeatWrap() = 0;
  virtual ITexture *setMirrorWrap() = 0;
  virtual ITexture *setClampWrap() = 0;
  virtual ITexture *setColorWrap(const glm::vec4 &color) = 0;
  virtual ITexture *setLineFilter() = 0;
  virtual ITexture *setNearestFilter() = 0;
};
}; // namespace cube::render