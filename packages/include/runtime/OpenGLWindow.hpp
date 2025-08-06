#pragma once
#include "Window.hpp"
#include "render/IRenderer.hpp"
namespace cube::runtime {
class OpenGLWindow : public Window {
private:
  SDL_GLContext _glContext = nullptr;
  render::IRenderer *_renderer = nullptr;

private:
  SDL_Window *createWindow();

public:
  OpenGLWindow();
  ~OpenGLWindow() override;
  void initOpenGLContext(int32_t major, int32_t minor);
  void present() override;
  render::IRenderer *getRenderer() override;
};
} // namespace cube::runtime