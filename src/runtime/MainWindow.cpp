#include "runtime/MainWindow.hpp"
#include "core/Error.hpp"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>
using namespace cube;
using namespace cube::runtime;
MainWindow::MainWindow(const std::string &title, uint32_t width,
                       uint32_t height, SDL_WindowFlags flags)
    : Window(title, width, height, flags) {
  _renderer = SDL_CreateRenderer(getHandle(), NULL);
  if (!_renderer) {
    throw core::Error("Failed to create renderer: {}", SDL_GetError());
  }
  SDL_SetRenderDrawColorFloat(_renderer, 0.2, 0.3, 0.3, 1.0);
}
MainWindow::~MainWindow() {
  if (_renderer) {
    SDL_DestroyRenderer(_renderer);
    _renderer = nullptr;
  }
}
auto MainWindow::onUpdate() -> void {
  SDL_RenderClear(_renderer);
  SDL_RenderPresent(_renderer);
}