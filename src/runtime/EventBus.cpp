#include "runtime/EventBus.hpp"
#include "core/Object.hpp"
#include "runtime/Application.hpp"
#include <algorithm>
#include <shared_mutex>
#include <vector>
using namespace cube;
using namespace cube::runtime;
std::shared_ptr<EventBus::Subscribe>
EventBus::subscribe(const std::string &type,
                    const std::function<void(core::Object *)> &callback) {
  static uint32_t idx = 1;
  std::unique_lock<std::shared_mutex> lock(_mutex);
  _callbacks[idx] = callback;
  _subscribes[type].push_back(idx);
  return std::make_shared<Subscribe>(type, idx++);
}

void EventBus::unsubscribe(const std::string &type, uint32_t id) {
  std::unique_lock<std::shared_mutex> lock(_mutex);
  if (_callbacks.contains(id)) {
    _callbacks.erase(id);
  }
  if (_subscribes.contains(type)) {
    auto &callbacks = _subscribes.at(type);
    auto it = std::find(callbacks.begin(), callbacks.end(), id);
    if (it != callbacks.end()) {
      callbacks.erase(it);
    }
  }
}

EventBus::Subscribe::Subscribe(const std::string &type, uint32_t id)
    : _type(type), _id(id) {}

void EventBus::Subscribe::cancel() {
  if (_id != 0) {
    auto &bus = Application::getInstance().getEventBus();
    bus.unsubscribe(_type, _id);
    _id = 0;
  }
}

EventBus::Subscribe::~Subscribe() { cancel(); }