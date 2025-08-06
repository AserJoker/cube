#include "runtime/Window.hpp"
#include "SDL3/SDL_video.h"
namespace cube::runtime {
Window::Window(SDL_Window *handle) : _handle(handle) {}
Window::~Window() {
  if (_handle) {
    SDL_DestroyWindow(_handle);
    _handle = nullptr;
  }
}
void Window::setTitle(const std::string &title) {
  if (_handle) {
    SDL_SetWindowTitle(_handle, title.c_str());
  }
}
void Window::setSize(uint32_t width, uint32_t height) {
  if (_handle) {
    SDL_SetWindowSize(_handle, width, height);
  }
}
void Window::setPosition(int32_t x, int32_t y) {
  if (_handle) {
    SDL_SetWindowPosition(_handle, x, y);
  }
}
} // namespace cube::runtime