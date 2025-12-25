#if RENDER_MODE == 1
#pragma once
#include "runtime/Event.hpp"
#include "runtime/System.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <queue>

namespace cube::runtime {
class System_SDL : public System {
private:
  SDL_Window *_window = nullptr;
  std::queue<std::shared_ptr<Event>> _events;

public:
  System_SDL();
  ~System_SDL() override;
  std::shared_ptr<Event> recvEvent() override;
  void sendEvent(const std::shared_ptr<Event> &event) override;
};
}; // namespace cube::runtime
#endif