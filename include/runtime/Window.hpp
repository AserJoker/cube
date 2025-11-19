#pragma once
#include "core/Object.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include <cstdint>
#include <string>
namespace cube::runtime {
class Window : public core::Object {
private:
  SDL_Window *_handle = nullptr;

public:
  Window(const std::string &title, uint32_t width, uint32_t height,
         SDL_WindowFlags flag = 0);
  ~Window() override;
  auto getID() const -> SDL_WindowID;
  auto getHandle() const -> SDL_Window *;
  virtual auto onUpdate() -> void;
};
} // namespace cube::runtime