#pragma once
#include "core/Object.hpp"
#include <glm/glm.hpp>
namespace cube::render {
class ICamera : public core::Object {
public:
  virtual const glm::mat4 &getProjection() const = 0;
  virtual const glm::mat4 &getView() const = 0;
};
} // namespace cube::render