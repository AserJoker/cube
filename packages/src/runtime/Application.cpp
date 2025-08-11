#include "runtime/Application.hpp"
#include "SDL3/SDL_events.h"
#include "core/ILogger.hpp"
#include "core/Logger.hpp"
#include "core/Singleton.hpp"
#include "runtime/EventInitialize.hpp"
#include "runtime/EventUninitialize.hpp"
#include "runtime/EventUpdate.hpp"
#include "runtime/EventWindowClose.hpp"
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
  _eventBus->emit<EventUpdate>();
  processEvent();
}

void Application::onWindowClose() {
  delete _window;
  _window = nullptr;
  _running = false;
}

void Application::processEvent() {
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
      auto e = _eventBus->emit<EventWindowClose>();
      if (e.isDefault()) {
        onWindowClose();
      }
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