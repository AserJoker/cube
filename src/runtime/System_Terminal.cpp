#if RENDER_MODE == 0
#include "runtime/System_Terminal.hpp"
using namespace cube;
using namespace cube::runtime;

System_Terminal::System_Terminal() {}
System_Terminal::~System_Terminal() {}
std::shared_ptr<Event> System_Terminal::recvEvent() {
  if (!_events.empty()) {
    auto event = _events.front();
    _events.pop();
    return event;
  }
  return nullptr;
}
void System_Terminal::sendEvent(const std::shared_ptr<Event> &event) {
  _events.push(event);
}
#endif