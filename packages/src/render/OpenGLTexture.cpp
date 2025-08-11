#include "render/OpenGLTexture.hpp"
#include "render/ITexture.hpp"
#include <glad/glad.h>
namespace cube::render {
OpenGLTexture::OpenGLTexture() { glGenTextures(1, &_handle); }
OpenGLTexture::~OpenGLTexture() {
  if (_handle) {
    glDeleteTextures(1, &_handle);
    _handle = 0;
  }
}
void OpenGLTexture::setImage(uint32_t width, uint32_t height,
                             const ITexture::FORMAT &fmt, void *data) {
  GLenum format;
  uint8_t size;
  switch (fmt) {
  case ITexture::FORMAT::RGB24:
    size = 3;
    format = GL_RGB;
    break;
  case ITexture::FORMAT::RGBA32:
    size = 4;
    format = GL_RGBA;
    break;
  case ITexture::FORMAT::GRAY:
    size = 1;
    format = GL_RED;
    break;
  }
  glBindTexture(GL_TEXTURE_2D, _handle);
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}
uint32_t OpenGLTexture::getHandle() const { return _handle; }
ITexture *OpenGLTexture::setRepeatWrap() {
  glBindTexture(GL_TEXTURE_2D, _handle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glBindTexture(GL_TEXTURE_2D, 0);
  return this;
}
ITexture *OpenGLTexture::setMirrorWrap() {
  glBindTexture(GL_TEXTURE_2D, _handle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glBindTexture(GL_TEXTURE_2D, 0);
  return this;
}
ITexture *OpenGLTexture::setClampWrap() {
  glBindTexture(GL_TEXTURE_2D, _handle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D, 0);
  return this;
}
ITexture *OpenGLTexture::setColorWrap(const glm::vec4 &color) {
  glBindTexture(GL_TEXTURE_2D, _handle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &color[0]);
  glBindTexture(GL_TEXTURE_2D, 0);
  return this;
}
ITexture *OpenGLTexture::setLineFilter() {
  glBindTexture(GL_TEXTURE_2D, _handle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
  return this;
}
ITexture *OpenGLTexture::setNearestFilter() {
  glBindTexture(GL_TEXTURE_2D, _handle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);
  return this;
}
} // namespace cube::render