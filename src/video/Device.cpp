#include "video/Device.hpp"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_gpu.h>
#include <stdexcept>
using namespace cube;
using namespace cube::video;
Device::Device(SDL_Window *window) : _window(window) {
  _device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, false, NULL);
  if (!_device) {
    throw std::runtime_error(std::string("Failed to create GPU Device: ") +
                             SDL_GetError());
  }
  if (!SDL_ClaimWindowForGPUDevice(_device, window)) {
    throw std::runtime_error(
        std::string("Failed to claim window for GPU Device: ") +
        SDL_GetError());
  }
}
Device::~Device() {
  if (_device) {
    SDL_ReleaseWindowFromGPUDevice(_device, _window);
    SDL_DestroyGPUDevice(_device);
    _device = nullptr;
  }
}