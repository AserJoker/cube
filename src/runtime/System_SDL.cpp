#if RENDER_MODE == 1
#include "runtime/System_SDL.hpp"
#include "core/Error.hpp"
#include "core/Logger.hpp"
#include "runtime/Application.hpp"
#include "runtime/Event.hpp"
#include <SDL3_ttf/SDL_ttf.h>
#include <memory>

using namespace cube;
using namespace cube::runtime;

static auto logHook(core::Logger *logger, int category,
                    SDL_LogPriority priority, const char *message) -> void {
  switch (priority) {
  case SDL_LOG_PRIORITY_INVALID:
  case SDL_LOG_PRIORITY_TRACE:
  case SDL_LOG_PRIORITY_VERBOSE:
  case SDL_LOG_PRIORITY_DEBUG:
    logger->debug("{}", message);
    break;
  case SDL_LOG_PRIORITY_INFO:
    logger->info("{}", message);
    break;
  case SDL_LOG_PRIORITY_WARN:
    logger->warn("{}", message);
    break;
  case SDL_LOG_PRIORITY_ERROR:
  case SDL_LOG_PRIORITY_CRITICAL:
    logger->error("{}", message);
    break;
  case SDL_LOG_PRIORITY_COUNT:
    logger->debug("{}", message);
    break;
  }
}
System_SDL::System_SDL() {
  auto &app = Application::getInstance();
  SDL_SetAppMetadata(app.getName().c_str(),
                     core::Version::serialize(app.getVersion()).c_str(), NULL);
  auto &logger = app.getLogger();
  SDL_SetLogOutputFunction((SDL_LogOutputFunction)logHook, &logger);
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
    throw core::Error("Failed to initialize sdl: {}", SDL_GetError());
  }
  if (!TTF_Init()) {
    throw core::Error("Failed to initialize sdl ttf: {}", SDL_GetError());
  }
  _window = SDL_CreateWindow(app.getName().c_str(), 1024, 768,
                             SDL_WINDOW_HIGH_PIXEL_DENSITY);
}
System_SDL::~System_SDL() {
  SDL_DestroyWindow(_window);
  _window = nullptr;
  TTF_Quit();
  SDL_Quit();
}

struct SDL_UserEventData {
  std::shared_ptr<Event> event;
};

std::shared_ptr<Event> System_SDL::recvEvent() {
  SDL_Event e;
  if (SDL_PollEvent(&e)) {
    if (e.type == SDL_EVENT_QUIT) {
      _events.push(std::make_shared<QuitEvent>());
    }
  }
  if (!_events.empty()) {
    auto event = _events.front();
    _events.pop();
    return event;
  }
  return nullptr;
}
void System_SDL::sendEvent(const std::shared_ptr<Event> &event) {
  _events.push(event);
}
#endif