#include "runtime/Window.hpp"
#include "SDL3/SDL_video.h"
#include <SDL3/SDL_messagebox.h>
#include <SDL3/SDL_mouse.h>
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
const char *Window::getTitle() const {
  if (_handle) {
    return SDL_GetWindowTitle(_handle);
  }
  return nullptr;
}

void Window::setSize(uint32_t width, uint32_t height) {
  if (_handle) {
    SDL_SetWindowSize(_handle, width, height);
  }
}
std::tuple<uint32_t, uint32_t> Window::getSize() const {
  int w, h;
  if (_handle) {
    SDL_GetWindowSize(_handle, &w, &h);
    return {w, h};
  }
  return {};
};
void Window::setPosition(int32_t x, int32_t y) {
  if (_handle) {
    SDL_SetWindowPosition(_handle, x, y);
  }
}
std::tuple<int32_t, int32_t> Window::getPosition() const {
  if (_handle) {
    int x, y;
    SDL_GetWindowPosition(_handle, &x, &y);
  }
  return {};
}
void Window::captureMouse() { SDL_SetWindowRelativeMouseMode(_handle, true); }

void Window::uncaptureMouse() {
  SDL_SetWindowRelativeMouseMode(_handle, false);
}
bool Window::isCaptureMouse() {
  return SDL_GetWindowRelativeMouseMode(_handle);
}

const std::string &Window::information(const std::string &message,
                                       const std::vector<std::string> &buttons,
                                       const std::string &title) {
  std::vector<SDL_MessageBoxButtonData> btns;
  for (auto &btn : buttons) {
    btns.push_back(
        {.flags = 0, .buttonID = (int)btns.size(), .text = btn.c_str()});
  }
  int id = 0;
  SDL_MessageBoxData data;
  data.message = message.c_str();
  data.title = title.c_str();
  data.window = _handle;
  data.numbuttons = btns.size();
  data.buttons = btns.data();
  data.colorScheme = nullptr;
  data.flags =
      SDL_MESSAGEBOX_INFORMATION | SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT;
  SDL_ShowMessageBox(&data, &id);
  return buttons[id];
};

const std::string &Window::confirm(const std::string &message,
                                   const std::vector<std::string> &buttons,
                                   const std::string &title) {
  return information(message, buttons, title);
};

const std::string &Window::error(const std::string &message,
                                 const std::vector<std::string> &buttons,
                                 const std::string &title) {
  std::vector<SDL_MessageBoxButtonData> btns;
  for (auto &btn : buttons) {
    btns.push_back(
        {.flags = 0, .buttonID = (int)btns.size(), .text = btn.c_str()});
  }
  int id = 0;
  SDL_MessageBoxData data;
  data.message = message.c_str();
  data.title = title.c_str();
  data.window = _handle;
  data.numbuttons = btns.size();
  data.buttons = btns.data();
  data.colorScheme = nullptr;
  data.flags = SDL_MESSAGEBOX_ERROR | SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT;
  SDL_ShowMessageBox(&data, &id);
  return buttons[id];
};

const std::string &Window::warn(const std::string &message,
                                const std::vector<std::string> &buttons,
                                const std::string &title) {
  std::vector<SDL_MessageBoxButtonData> btns;
  for (auto &btn : buttons) {
    btns.push_back(
        {.flags = 0, .buttonID = (int)btns.size(), .text = btn.c_str()});
  }
  int id = 0;
  SDL_MessageBoxData data;
  data.message = message.c_str();
  data.title = title.c_str();
  data.window = _handle;
  data.numbuttons = btns.size();
  data.buttons = btns.data();
  data.colorScheme = nullptr;
  data.flags = SDL_MESSAGEBOX_WARNING | SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT;
  SDL_ShowMessageBox(&data, &id);
  return buttons[id];
};
} // namespace cube::runtime