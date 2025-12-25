#pragma once
#include "Event.hpp"
#include "core/Object.hpp"
#include <memory>
namespace cube::runtime {
class System : public core::Object {
public:
  virtual std::shared_ptr<Event> recvEvent() = 0;
  virtual void sendEvent(const std::shared_ptr<Event> &event) = 0;
};
}; // namespace cube::runtime