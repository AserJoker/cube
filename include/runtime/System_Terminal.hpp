#if RENDER_MODE == 0
#pragma once
#include "runtime/System.hpp"
#include <queue>
namespace cube::runtime {
class System_Terminal : public System {
private:
  std::queue<std::shared_ptr<Event>> _events;

public:
  System_Terminal();
  ~System_Terminal() override;
  std::shared_ptr<Event> recvEvent() override;
  void sendEvent(const std::shared_ptr<Event> &event) override;
};
} // namespace cube::runtime
#endif