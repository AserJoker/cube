#pragma once
#include "IWindow.hpp"
#include <SDL3/SDL.h>
#include <string>

namespace cube::runtime {
class Window : public IWindow {
private:
  SDL_Window *_handle;

public:
  Window(SDL_Window *handle = nullptr);
  ~Window() override;
  SDL_Window *getHandle() { return _handle; }
  void setTitle(const std::string &title) override;
  void setSize(uint32_t width, uint32_t height) override;
  void setPosition(int32_t x, int32_t y) override;
};
} // namespace cube::runtime