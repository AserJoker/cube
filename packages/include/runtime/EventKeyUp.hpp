#pragma once
#include "core/Event.hpp"
#include <cstdint>
namespace cube::runtime {
class EventKeyUp : public core::Event<"cube.runtime.KeyUp"> {
private:
  uint32_t _code;

public:
  EventKeyUp(uint32_t code) : _code(code) {}
  inline uint32_t getCode() const { return _code; }
};
} // namespace cube::runtime