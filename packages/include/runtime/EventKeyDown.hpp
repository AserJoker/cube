#pragma once
#include "core/Event.hpp"
#include <cstdint>
namespace cube::runtime {
class EventKeyDown : public core::Event<"cube.runtime.KeyDown"> {
private:
  uint32_t _code;

public:
  EventKeyDown(uint32_t code) : _code(code) {}
  inline uint32_t getCode() const { return _code; }
};
} // namespace cube::render