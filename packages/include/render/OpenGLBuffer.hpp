#pragma once
#include "IBuffer.hpp"
#include <cstdint>
namespace cube::render {
class OpenGLBuffer : public IBuffer {
private:
  uint32_t _handle;

public:
  OpenGLBuffer(size_t size = 0, void *data = 0);
  ~OpenGLBuffer() override;
  uint32_t getHandle() const;

  void reset(size_t size, void *data = 0) override;
  void write(void *data, size_t offset, size_t size) override;
  size_t getSize() const override;
};
} // namespace cube::render