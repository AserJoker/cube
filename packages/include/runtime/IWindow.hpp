#pragma once
#include "core/Object.hpp"
#include "render/IRenderer.hpp"
#include <string>
namespace cube::runtime {
class IWindow : public core::Object {
public:
  virtual render::IRenderer *getRenderer() = 0;
  virtual void setTitle(const std::string &title) = 0;
  virtual void setPosition(int32_t x, int32_t y) = 0;
  virtual void setSize(uint32_t width, uint32_t height) = 0;
  virtual void present() = 0;
};
} // namespace cube::runtime