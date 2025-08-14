#pragma once
#include "core/Event.hpp"
#include <cstdint>
#include <glm/glm.hpp>
namespace cube::runtime {
class EventMouseButtonDown
    : public core::Event<"cube.runtime.MouseButtonDown"> {
private:
  uint8_t _button;
  glm::ivec2 _position;

public:
  EventMouseButtonDown(uint8_t btn, const glm::ivec2 &position)
      : _button(btn), _position(position) {};
  inline uint32_t getButton() const { return _button; }
  inline const glm::ivec2 &getPosition() const { return _position; }
};
} // namespace cube::runtime