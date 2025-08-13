#pragma once
#include "core/Object.hpp"
namespace cube::render {
class IBuffer : public core::Object {
public:
  enum class Access { READ_ONLY, WRITE_ONLY, READ_WRITE };

public:
  virtual void reset(size_t size, void *data = 0) = 0;
  virtual void write(void *data, size_t offset, size_t size) = 0;
  virtual size_t getSize() const = 0;
  virtual void lock(void **ppOutput,
                    const Access &access = IBuffer::Access::READ_WRITE) = 0;
  virtual void unlock() = 0;
};
} // namespace cube::render