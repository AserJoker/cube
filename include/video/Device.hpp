#ifndef _H_CUBE_VIDEO_DEVICE_
#define _H_CUBE_VIDEO_DEVICE_
#include "core/Instance.hpp"
#include <SDL3/SDL.h>
namespace cube::video {
class Device : public core::Instance {
private:
  SDL_GPUDevice *_device = nullptr;
  SDL_Window *_window = nullptr;

public:
  Device(SDL_Window *window);
  ~Device() override;
};
} // namespace cube::video
#endif