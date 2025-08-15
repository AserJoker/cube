#include "runtime/Application.hpp"
#include "core/ILogger.hpp"
#include "core/Logger.hpp"
#include "core/Singleton.hpp"
#include "runtime/EventInitialize.hpp"
#include "runtime/EventKeyDown.hpp"
#include "runtime/EventKeyUp.hpp"
#include "runtime/EventMouseButtonDown.hpp"
#include "runtime/EventMouseButtonUp.hpp"
#include "runtime/EventMouseMove.hpp"
#include "runtime/EventMouseWheel.hpp"
#include "runtime/EventSDL.hpp"
#include "runtime/EventUninitialize.hpp"
#include "runtime/EventUpdate.hpp"
#include "runtime/EventWindowClose.hpp"
#include "runtime/EventWindowFocus.hpp"
#include "runtime/EventWindowFocusLost.hpp"
#include "runtime/OpenGLWindow.hpp"
#include <SDL3/SDL.h>
#include <exception>
#include <iostream>

namespace cube::runtime {
Application *Application::_instance = nullptr;
Application::Application(int argc, char *argv[]) {
  _instance = this;
  for (int idx = 0; idx < argc; idx++) {
    _argv.emplace_back(argv[idx]);
  }
  if (!_logger) {
    _logger = core::Singleton<core::ILogger, false>::initialize<core::Logger>();
  }
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
}

Application::~Application() {
  SDL_Quit();
  _instance = nullptr;
}

const std::vector<std::string> &Application::getArgv() const { return _argv; }

void Application::runTick() {
  static auto time = SDL_GetTicks();
  auto now = SDL_GetTicks();
  _eventBus->emit<EventUpdate>(now + 1 - time);
  processEvent();
  time = SDL_GetTicks();
}

void Application::onWindowClose() {
  delete _window;
  _window = nullptr;
  _running = false;
}

void Application::processEvent() {
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    auto e = _eventBus->emit<EventSDL>(event);
    if (!e.isDefault()) {
      return;
    }
    if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
      auto e = _eventBus->emit<EventWindowClose>(_window);
      if (e.isDefault()) {
        onWindowClose();
      }
    } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
      _eventBus->emit<EventMouseButtonDown>(
          event.button.button, glm::ivec2{event.button.x, event.button.y});
    } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
      _eventBus->emit<EventMouseButtonUp>(
          event.button.button, glm::ivec2{event.button.x, event.button.y});
    } else if (event.type == SDL_EVENT_MOUSE_MOTION) {
      _eventBus->emit<EventMouseMove>(
          glm::ivec2{event.motion.xrel, event.motion.yrel});
    } else if (event.type == SDL_EVENT_MOUSE_WHEEL) {
      _eventBus->emit<EventMouseWheel>(glm::vec2(event.wheel.x, event.wheel.y));
    } else if (event.type == SDL_EVENT_KEY_DOWN) {
      _eventBus->emit<EventKeyDown>(event.key.key);
    } else if (event.type == SDL_EVENT_KEY_UP) {
      _eventBus->emit<EventKeyUp>(event.key.key);
    } else if (event.type == SDL_EVENT_WINDOW_FOCUS_GAINED) {
      _eventBus->emit<EventWindowFocus>(_window);
    } else if (event.type == SDL_EVENT_WINDOW_FOCUS_LOST) {
      _eventBus->emit<EventWindowFocusLost>(_window);
    }
  }
}

int32_t Application::run() {
  try {
    _eventBus->emit<EventInitialize>();
    if (!_window) {
      this->_running = false;
    }
    while (this->_running) {
      runTick();
    }
    _eventBus->emit<EventUninitialize>();
    return 0;
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }
}
void Application::exit() { _running = false; }

IWindow *Application::createOpenGLWindow(int32_t major, int32_t minor) {
  auto win = create<OpenGLWindow>();
  win->initOpenGLContext(major, minor);
  _window = win;
  return _window;
}
} // namespace cube::runtime