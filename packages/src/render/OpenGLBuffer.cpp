#include "render/OpenGLBuffer.hpp"
#include <glad/glad.h>
namespace cube::render {
OpenGLBuffer::OpenGLBuffer(size_t size, void *data) : _handle(0) {
  glGenBuffers(1, &_handle);
  if (size) {
    reset(size, data);
  }
}
OpenGLBuffer::~OpenGLBuffer() {
  if (_handle) {
    glDeleteBuffers(1, &_handle);
  }
}

void OpenGLBuffer::reset(size_t size, void *data) {
  glBindBuffer(GL_ARRAY_BUFFER, _handle);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLBuffer::write(void *data, size_t offset, size_t size) {
  glBindBuffer(GL_ARRAY_BUFFER, _handle);
  glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

uint32_t OpenGLBuffer::getHandle() const { return _handle; }

size_t OpenGLBuffer::getSize() const {
  int32_t size = 0;
  glBindBuffer(GL_ARRAY_BUFFER, _handle);
  glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return size;
}
void OpenGLBuffer::lock(void **ppOutput, const Access &access) {
  GLenum glAccess;
  switch (access) {
  case IBuffer::Access::READ_ONLY:
    glAccess = GL_READ_ONLY;
    break;
  case IBuffer::Access::WRITE_ONLY:
    glAccess = GL_WRITE_ONLY;
    break;
  case IBuffer::Access::READ_WRITE:
    glAccess = GL_READ_WRITE;
    break;
  }
  glBindBuffer(GL_ARRAY_BUFFER, _handle);
  *ppOutput = glMapBuffer(GL_ARRAY_BUFFER, glAccess);
};
void OpenGLBuffer::unlock() { glUnmapBuffer(GL_ARRAY_BUFFER); };
}; // namespace cube::render