#pragma once
#include "Window.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
namespace cube::runtime {
class MainWindow : public Window {
private:
  SDL_Renderer *_renderer = nullptr;

public:
  MainWindow(const std::string &title, uint32_t width, uint32_t height,
             SDL_WindowFlags flags = 0);
  ~MainWindow() override;
  auto onUpdate() -> void override;
};
} // namespace cube::runtime