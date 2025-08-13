#pragma once
#include "IAttribute.hpp"
#include "render/OpenGLBuffer.hpp"
#include <cstdint>
namespace cube::render {
class OpenGLAttribute : public IAttribute {
private:
  uint32_t _stride;
  bool _normalize;
  OpenGLBuffer *_buffer;

public:
  OpenGLAttribute(uint32_t stride, bool normalize);
  void reset(size_t size, void *data = 0) override;
  void write(void *data, size_t offset, size_t size) override;
  size_t getSize() const override;
  void lock(void **ppOutput,
            const Access &access = IBuffer::Access::READ_WRITE) override;
  void unlock() override;
  uint32_t getStride() const override;
  bool isNormalize() const override;
  uint32_t getHandle() const;
};
} // namespace cube::render