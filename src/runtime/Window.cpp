#include "runtime/Window.hpp"
#include "core/Error.hpp"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_video.h>
using namespace cube;
using namespace cube::runtime;
Window::Window(const std::string &title, uint32_t width, uint32_t height,
               SDL_WindowFlags flags) {
  _handle = SDL_CreateWindow(title.c_str(), width, height, flags);
  if (!_handle) {
    throw core::Error("Failed to create window: {}", SDL_GetError());
  }
}
Window::~Window() {
  if (_handle) {
    SDL_DestroyWindow(_handle);
    _handle = nullptr;
  }
}

auto Window::getID() const -> SDL_WindowID { return SDL_GetWindowID(_handle); }

auto Window::getHandle() const -> SDL_Window * { return _handle; }

auto Window::onUpdate() -> void {}