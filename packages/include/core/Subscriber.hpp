#pragma once
#include "EventBus.hpp"
#include "Object.hpp"
#include "core/Singleton.hpp"
namespace cube::core {
class Subscriber : public Object {
protected:
  EventBus *_eventBus = Singleton<EventBus>::get();

public:
  ~Subscriber() override { _eventBus->off(this); };
};
} // namespace cube::core