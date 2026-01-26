#include "core/Object.hpp"
#include <SDL3/SDL.h>
namespace cube::video {
class Device : public core::Object {
private:
  SDL_GPUDevice *_device = nullptr;
  SDL_Window *_window = nullptr;

public:
  Device(SDL_Window *window);
  ~Device() override;
};
} // namespace cube::video