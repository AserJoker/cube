#pragma once
#include "core/Event.hpp"
#include <cstdint>
namespace cube::runtime {
class EventUpdate : public core::Event<"cube.runtime.Update"> {
private:
  uint64_t _tick;

public:
  EventUpdate(uint64_t tick) : _tick(tick) {}
  inline uint64_t getTick() const { return _tick; }
};
} // namespace cube::runtime