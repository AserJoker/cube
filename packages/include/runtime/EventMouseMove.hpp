#pragma once
#include "core/Event.hpp"
#include <glm/glm.hpp>
namespace cube::runtime {
class EventMouseMove : public core::Event<"cube.runtime.EventMouseMove"> {
private:
  glm::ivec2 _motion;

public:
  EventMouseMove(const glm::ivec2 &motion) : _motion(motion) {};
  const glm::ivec2 &getMotion() const { return _motion; }
};
} // namespace cube::runtime