#pragma once
#include "core/Event.hpp"
#include <glm/glm.hpp>
namespace cube::runtime {
class EventMouseWheel : public core::Event<"cube.runtime.MouseWheel"> {
private:
  glm::vec2 _delta;

public:
  EventMouseWheel(const glm::vec2 &delta) : _delta(delta) {}
  inline const glm::vec2 &getDelta() const { return _delta; }
};
}; // namespace cube::runtime