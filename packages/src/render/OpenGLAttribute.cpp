#include "render/OpenGLAttribute.hpp"
#include "render/OpenGLBuffer.hpp"
namespace cube::render {
OpenGLAttribute::OpenGLAttribute(uint32_t stride, bool normalize)
    : _stride(stride), _normalize(normalize) {
  _buffer = create<OpenGLBuffer>();
}
void OpenGLAttribute::reset(size_t size, void *data) {
  _buffer->reset(size, data);
};
void OpenGLAttribute::write(void *data, size_t offset, size_t size) {
  _buffer->write(data, offset, size);
}
size_t OpenGLAttribute::getSize() const { return _buffer->getSize(); };
void OpenGLAttribute::lock(void **ppOutput, const Access &access) {
  _buffer->lock(ppOutput, access);
};
void OpenGLAttribute::unlock() { _buffer->unlock(); };
uint32_t OpenGLAttribute::getStride() const { return _stride; };
bool OpenGLAttribute::isNormalize() const { return _normalize; };
uint32_t OpenGLAttribute::getHandle() const { return _buffer->getHandle(); };
} // namespace cube::render