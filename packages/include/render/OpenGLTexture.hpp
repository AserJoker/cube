#pragma once
#include "ITexture.hpp"
namespace cube::render {
class OpenGLTexture : public ITexture {
private:
  uint32_t _handle;

public:
  OpenGLTexture();
  ~OpenGLTexture() override;
  void setImage(uint32_t width, uint32_t height, const ITexture::FORMAT &fmt,
                void *data);
  uint32_t getHandle() const;

  ITexture *setRepeatWrap() override;
  ITexture *setMirrorWrap() override;
  ITexture *setClampWrap() override;
  ITexture *setColorWrap(const glm::vec4 &color) override;
  ITexture *setLineFilter() override;
  ITexture *setNearestFilter() override;
};
}; // namespace cube::render