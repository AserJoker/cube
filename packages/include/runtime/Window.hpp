#pragma once
#include "IWindow.hpp"
#include <SDL3/SDL.h>
#include <string>

#ifndef CUBE_DEFAULT_WINDOW_WIDTH
#define CUBE_DEFAULT_WINDOW_WIDTH 1024
#endif

#ifndef CUBE_DEFAULT_WINDOW_HEIGHT
#define CUBE_DEFAULT_WINDOW_HEIGHT 768
#endif

namespace cube::runtime {
class Window : public IWindow {
private:
  SDL_Window *_handle;

public:
  Window(SDL_Window *handle = nullptr);
  ~Window() override;
  SDL_Window *getHandle() { return _handle; }
  void setTitle(const std::string &title) override;
  const char *getTitle() const override;
  void setSize(uint32_t width, uint32_t height) override;
  std::tuple<uint32_t, uint32_t> getSize() const override;
  void setPosition(int32_t x, int32_t y) override;
  std::tuple<int32_t, int32_t> getPosition() const;
  const std::string &
  information(const std::string &message,
              const std::vector<std::string> &buttons = {"Ok"},
              const std::string &title = "information") override;
  const std::string &
  confirm(const std::string &message,
          const std::vector<std::string> &buttons = {"Ok", "Cancel"},
          const std::string &title = "confirm") override;
  const std::string &error(const std::string &message,
                           const std::vector<std::string> &buttons = {"Ok"},
                           const std::string &title = "error") override;
  const std::string &warn(const std::string &message,
                          const std::vector<std::string> &buttons = {"Ok"},
                          const std::string &title = "warn") override;
};
} // namespace cube::runtime